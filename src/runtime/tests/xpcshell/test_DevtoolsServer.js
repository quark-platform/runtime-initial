/* Any copyright is dedicated to the Public Domain.
http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const { DevtoolsServer } = ChromeUtils.import(
  "resource://runtime/modules/DevtoolsServer.jsm"
);

function run_test() {
  const devtools = DevtoolsServer.get(1234, true);

  ok(devtools.port === 1234, "The defined port is set");

  let error = false;

  try {
    new DevtoolsServer();
  } catch (e) {
    error = true;
  }

  ok(error, "DevtoolsServer is a singleton, should not be constructed");
}
