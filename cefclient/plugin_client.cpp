// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_nplugin.h"
#include "cefclient/clientplugin.h"

void InitPluginClient() {
  // Structure providing information about the client plugin.
  CefPluginInfo plugin_info;
  CefString(&plugin_info.display_name).FromASCII("Client Window Plugin");
  CefString(&plugin_info.unique_name).FromASCII("client_window_plugin");
  CefString(&plugin_info.description).FromASCII("Window Client Plugin");
  CefString(&plugin_info.mime_types).FromASCII("application/x-window-plugin");
  CefString(&plugin_info.file_extensions).FromASCII("client");

  plugin_info.np_getentrypoints = NP_ClientGetEntryPoints;
  plugin_info.np_initialize = NP_ClientInitialize;
  plugin_info.np_shutdown = NP_ClientShutdown;

  // Register the internal client plugin
  CefRegisterPlugin(plugin_info);
}