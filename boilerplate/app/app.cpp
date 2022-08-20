#include <stdio.h>
#include <cstdio>

#include "mozilla/Bootstrap.h"
#include "BinaryPath.h"
#include "XREShellData.h"
#include "application.ini.h"
#include "mozilla/CmdLineAndEnvUtils.h"

using namespace mozilla;

static void printHelp(char* argv[]) {
  printf("Usage: %s [OPTIONS]\n", argv[0]);
  printf("\n");
  printf("Options:\n");
  printf("  -h, --help            Print this message\n");
  printf("  -v, --version         Print the application version\n");
  printf("  --app [app_file]      Runs an application.ini file\n");
  printf("\n");
  printf("Notes:\n");
  printf("The --app flag is required.\n");
}

static bool checkArgs(int argc, char* argv[]) {
  // To few arguments
  if (argc < 2) {
    printf("Not enough arguments\n\n");
    printHelp(argv);
    return false;
  }

  // If help, print help and exit
  if (ARG_FOUND == CheckArg(argc, argv, "h") ||
      ARG_FOUND == CheckArg(argc, argv, "help")) {
    printHelp(argv);
    return false;
  }

  // If version, print version and exit
  if (ARG_FOUND == CheckArg(argc, argv, "v") ||
      ARG_FOUND == CheckArg(argc, argv, "version")) {
    printf("%s\n", sAppData.version);
    return false;
  }

  // If no app, print help and exit
  if (ARG_NONE == CheckArg(argc, argv, "app", nullptr, CheckArgFlag::None)) {
    printf("No app specified\n\n");
    printHelp(argv);
    return false;
  }

  return true;
}

// From what I can tell, this stores the result of bootstrapping to avoid
// initing it multiple times
Bootstrap::UniquePtr gBootstrap;

static nsresult InitXPCOMGlue(LibLoadingStrategy aLibLoadingStrategy) {
  // If we have already bootstraped, we do not want to do it again
  if (gBootstrap) {
    return NS_OK;
  }

  UniqueFreePtr<char> exePath = BinaryPath::Get();
  if (!exePath) {
    printf("Couldn't fild the application directory.\n");
    return NS_ERROR_FAILURE;
  }

  // Load the XPCOM dynamic library
  auto bootstrapResult =
      mozilla::GetBootstrap(exePath.get(), aLibLoadingStrategy);
  if (bootstrapResult.isErr()) {
    printf("Couldn't load XPCOM.\n");
    return NS_ERROR_FAILURE;
  }

  gBootstrap = bootstrapResult.unwrap();

  // Start the main thread
  gBootstrap->NS_LogInit();

  return NS_OK;
}

// This is a renamed version of the firefox do_main function
static int runXREMain(int argc, char* argv[], char* envp[]) {
  BootstrapConfig config{};

  const char* appini = nullptr;
  if (ARG_FOUND == CheckArg(argc, argv, "app", &appini, CheckArgFlag::None)) {
    printf("Running appini: %s\n", appini);
    config.appData = nullptr;
    config.appDataPath = appini;
  } else {
    printf("No app specified\n\n");
  }

  // TODO: Do we need to add support for MOZ_SANDBOX on Windows
  // NOTE: We do not have access to lib fuzer

  // NOTE: FF needs to keep in sync with LauncherProcessWin,
  //       TB doesn't have that file.
  //       Nor does this template
  const char* acceptableParams[] = {};
  EnsureCommandlineSafe(argc, argv, acceptableParams);

  return gBootstrap->XRE_main(argc, argv, config);
}

int main(int argc, char* argv[], char* envp[]) {
  // Check if the arguments provided are correct
  if (!checkArgs(argc, argv)) {
    return 1;
  }

  // TODO: Is fork server important
  // TODO: Profiler logging
  // TODO: Content process
  // TODO: Has DLL Blocklist
  // TODO: Do we care about silent mode? What even is it?
  // TODO: XP_WIN High dpi support

  nsresult rv = InitXPCOMGlue(LibLoadingStrategy::NoReadAhead);
  if (NS_FAILED(rv)) {
    printf("InitXPCOMGlue failed\n");
    return 255;
  }

  // BootstrapConfig config{};

  int result = runXREMain(argc, argv, envp);

  gBootstrap->NS_LogTerm();

#ifdef XP_MACOSX
  // Allow writes again. While we would like to catch writes from static
  // destructors to allow early exits to use _exit, we know that there is
  // at least one such write that we don't control (see bug 826029). For
  // now we enable writes again and early exits will have to use exit instead
  // of _exit.
  gBootstrap->XRE_StopLateWriteChecks();
#endif

  gBootstrap.reset();
  return result;
}
