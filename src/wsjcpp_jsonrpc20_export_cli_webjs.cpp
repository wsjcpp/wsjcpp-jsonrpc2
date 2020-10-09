#include "wsjcpp_jsonrpc20_export_cli_webjs.h"

#include <wsjcpp_core.h>
#include <wsjcpp_jsonrpc20.h>
#include <fstream>

// ---------------------------------------------------------------------

WsjcppJsonRpc20ExportCliWebJs::WsjcppJsonRpc20ExportCliWebJs(
    const std::string &sExportDir,
    const std::string &sPackageName
) : WsjcppJsonRpc20ExportCliBase("cli-webjs", sExportDir, sPackageName) {
    TAG = "WsjcppJsonRpc20ExportCliWebJs";

    // TODO must be WsjcppJsonRpc20::eventsList() or something like
    m_vEvents.push_back("server");
    m_vEvents.push_back("notify");
    m_vEvents.push_back("chat");
    m_vEvents.push_back("connected");
    m_vEvents.push_back("reconnecting");
    m_vEvents.push_back("disconnected");
    m_vEvents.push_back("broken");
    m_vEvents.push_back("userdata");
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliWebJs::setIssuesURL(const std::string &sIssuesURL) {
    m_sIssuesURL = sIssuesURL;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliWebJs::setRepository(const std::string &sRepositoryType, const std::string &sRepositoryURL) {
    m_sRepositoryType = sRepositoryType;
    m_sRepositoryURL = sRepositoryURL;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliWebJs::setLicense(const std::string &sLicenseType, const std::string &sLicenseURL) {
    m_sLicenseType = sLicenseType;
    m_sLicenseURL = sLicenseURL;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::doExportLib() {
    std::string sBasicDir = "./" + this->getPackageName();

    if (!prepareDirs()) {
        return false;
    }
    if (!exportLibCliWebJSFile()) {
        return false;
    }

    // exportLibCliWebServiceTSFile(sBasicDir);
    if (!exportSampleHtmlFile()) {
        return false;
    }
    if (!exportPackageJson()) {
        return false;
    }
    if (!exportAPImd()) {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::prepareDirs() {
    std::vector<std::string> vDirs;
    vDirs.push_back(this->getExportDir());
    vDirs.push_back(this->getExportDir() + "/dist");
    
    for (int i = 0; i < vDirs.size(); i++) {
        std::string sDir = vDirs[i];
        if (!WsjcppCore::dirExists(sDir)) {
            
            if (WsjcppCore::makeDir(sDir)) {
                WsjcppLog::ok(TAG, "Created directory '" + sDir + "'");
            } else {
                WsjcppLog::err(TAG, "Could not create directory '" + sDir + "'");
                return false;
            }
        } else {
            WsjcppLog::info(TAG, "Directory already exists '" + sDir + "'");
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::exportPackageJson() {
    std::ofstream packageJson;
    std::string sFilename = this->getExportDir() + "/package.json";
    std::cout << " * write code to " << sFilename << std::endl;
    packageJson.open(sFilename);
    if (!packageJson.is_open()) {
        WsjcppLog::err(TAG, "Could not open file for write '" + sFilename + "'");
        return false;
    }

    // look here an example https://github.com/jquery/jquery/blob/master/package.json
    packageJson << 
        "{\n"
        "  \"name\": \"" + this->getPackageName() + "\",\n"
        "  \"version\": \"" + this->getAppVersion() + "\",\n"
        "  \"description\": \"" + this->getClassName() + " JavaScript Web Client Library for " + this->getAppName() + "\",\n"
        "  \"main\": \"dist/" + this->getPackageName() + ".js\",\n"
        "  \"repository\": {\n"
        "    \"type\": \"" + m_sRepositoryType + "\",\n"
        "    \"url\": \"" + m_sRepositoryURL + "\"\n"
        "  },\n";
    if (this->getKeywords().size() > 0) {
        packageJson << 
        "  \"keywords\": [\n    \"" + WsjcppCore::join(this->getKeywords(), "\",\n    \"") + "\"\n  ],\n";
    }
    packageJson << 
        "  \"bugs\": {\n"
        "    \"url\": \"" + m_sIssuesURL + "\"\n"
        "  },\n"
        "  \"author\": \"" + this->getAuthorName() + " <" + this->getAuthorEmail() + ">\",\n"
        "  \"license\": \"" + m_sLicenseType + "\",\n"
        "  \"licenses\": [{\n"
        "    \"type\": \"" + m_sLicenseType + "\",\n"
        "    \"url\": \"" + m_sLicenseURL + "\"\n"
        "  }]\n"
        "}\n";
    packageJson.close();
    std::cout << "\t> OK" << std::endl;

    return false;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::exportAPImd() {
    
    std::ofstream apimd;
    std::string sFilename = this->getExportDir() + "/API.md";
    std::cout << " * write file to " << sFilename << std::endl;
    apimd.open(sFilename);
    if (!apimd.is_open()) {
        WsjcppLog::err(TAG, "Could not open file for write '" + sFilename + "'");
        return false;
    }

    apimd <<
        "# " + this->getPackageName() + "\n\n"
        " Automatically generated by " << this->getAppName() << ". \n"
        " * Version: " << this->getAppVersion() << "\n"
        " * Date: " << this->getDateNow() << "\n\n"
        " Include script ```dist/" + this->getPackageName() + ".js```\n"
        " Example connect:\n"
        "```\n"
        "var client = new " + this->getClassName() + "();\n"
    ;
    for (int i = 0; i < m_vEvents.size(); i++) {
        apimd <<
            "client.bind('" + m_vEvents[i] + "', function(data) { console.log('" + m_vEvents[i] + "', data)})\n";
    }
    apimd <<
        "client.bind('connected', function(data) { console.log('connected', data)})\n"
        "// connect\n"
        "client.init({'baseUrl': '" + this->getDefaultConnectionString() + "'})\n"
        "// disconnect\n"
        "client.deinit()\n"
        "```\n"
        "\n";

    std::map<std::string, WsjcppJsonRpc20HandlerBase*>::iterator it = g_pWsjcppJsonRpc20HandlerList->begin();
    for (; it!=g_pWsjcppJsonRpc20HandlerList->end(); ++it) {
        std::string sCmd = it->first;
        WsjcppJsonRpc20HandlerBase* pWsjcppJsonRpc20HandlerBase = it->second;
        
        apimd <<
            "<details>\n"
            "<summary>" << sCmd << "</summary>\n\n"
            "## " << sCmd << "\n\n";

        if (pWsjcppJsonRpc20HandlerBase->getDescription() != "") {
            apimd << pWsjcppJsonRpc20HandlerBase->getDescription() << "\n\n";
        }
        apimd <<
            "Access: unauthorized - **" << (pWsjcppJsonRpc20HandlerBase->haveUnauthorizedAccess() ? "yes" : "no") << "**, "
            " user - **" << (pWsjcppJsonRpc20HandlerBase->haveUserAccess() ? "yes" : "no") << "**, "
            " tester - **" << (pWsjcppJsonRpc20HandlerBase->haveTesterAccess() ? "yes" : "no") << "**, "
            " admin - **" << (pWsjcppJsonRpc20HandlerBase->haveAdminAccess() ? "yes" : "no") << "**\n"
            "\n";

        apimd << " #### Input params \n\n";

        std::string jsTemplate = "";

        std::vector<WsjcppJsonRpc20ParamDef> vVin = pWsjcppJsonRpc20HandlerBase->getParamsDef();
        for (int i = 0; i < vVin.size(); i++) {
            WsjcppJsonRpc20ParamDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());

            apimd << " * " << inDef.getName() << " - " << inDef.getType() << ", " << inDef.getRestrict() << "; " << inDef.getDescription() << "\n";

            if (jsTemplate != "") {
                jsTemplate += ",\n";
            }
            if (inDef.isInteger()) {
                int nVal = 0;
                if (inDef.getName() == "onpage") {
                    nVal = 10;
                }
                jsTemplate += "    \"" + inDef.getName() + "\": " + std::to_string(nVal);
            } else {
                jsTemplate += "    \"" + inDef.getName() + "\": \"\"";
            }
        }
        apimd <<
            "\n\n"
            " #### example call method \n\n"
            "```\n"
            "client." + sCmd + "({\n" + jsTemplate + "\n}).done(function(r) {\n"
            "    console.log('Success: ', r);\n"
            "}).fail(function(err) {\n"
            "    console.error('Error:', err);\n"
            "});\n"
            "```"
            "\n\n"
            "</details>"
            "\n\n";
    }

    apimd.close();
    std::cout << "\t> OK" << std::endl;
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::exportSampleHtmlFile() {
    std::string sFilename = this->getExportDir() + "/sample.html";
    std::ofstream sample_html;
    std::cout << " * write code to " << sFilename << std::endl;
    sample_html.open(sFilename);
    if (!sample_html.is_open()) {
        WsjcppLog::err(TAG, "Could not open file for write '" + sFilename + "'");
        return false;
    }

    sample_html <<
        "<html>\r\n"
        "<head>\r\n"
        "    <script src='dist/" + this->getPackageName() + ".js'></script>\r\n"
        "    <script>\r\n"
        "        function log(n) {\r\n"
        "            console.log(n);\r\n"
        "            var log = document.getElementById('log');\r\n"
        "            log.innerHTML += '[' + new Date() + '] ' + n + '\\r\\n';\r\n"
        "        };\r\n"
        "        var client = new " + this->getClassName() + "();\r\n"
        "        document.addEventListener('DOMContentLoaded', function() {\r\n"
        "            client.bind('notify', function(data) {\r\n"
        "                log('notify: ' + JSON.stringify(data))\r\n"
        "            });\r\n"
        "            client.bind('connected', function(data) {\r\n"
        "                log('connected: ' + JSON.stringify(data))\r\n"
        "                connecting_form.style.display = 'none';\r\n"
        "                login_form.style.display = '';\r\n"
        "                logout_form.style.display = 'none';\r\n"
        "            });\r\n"
        "            client.bind('disconnected', function(data) {\r\n"
        "                log('disconnected: ' + JSON.stringify(data))\r\n"
        "                connecting_form.style.display = '';\r\n"
        "                login_form.style.display = 'none';\r\n"
        "                logout_form.style.display = 'none';\r\n"
        "            });\r\n"
        "            client.bind('userdata', function(data) {\r\n"
        "                log('userdata: ' + JSON.stringify(data))\r\n"
        "                login_form.style.display = 'none';\r\n"
        "                logout_form.style.display = '';\r\n"
        "            });\r\n"
        "        });\r\n"
        "    </script>\r\n"
        "</head>\r\n"
        "<body>\r\n"
        "<div id='connecting_form'>\r\n"
        "    <h1>Connect to Server</h1>\r\n"
        "    <input type='text' id='server_url'><br>\r\n"
        "    <button id='btn_connect'>Connect</button>\r\n"
        "</div>\r\n"
        "<div id='login_form' style='display: none'>\r\n"
        "    <h1>Login</h1>\r\n"
        "    <input type='text' id='login'><br>\r\n"
        "    <input type='password' id='password'><br>\r\n"
        "    <button id='btn_login'>Login</button>\r\n"
        "</div>\r\n"
        "<div id='logout_form' style='display: none'>\r\n"
        "    <h1>Connected</h1>\r\n"
        "    <button id='btn_logout'>Logout</button>\r\n"
        "</div>\r\n"
        "<script>\r\n"
        "    server_url.value = 'ws://' + window.location.hostname + ':1234/api-ws/'\r\n"
        "    btn_login.onclick = function() {\r\n"
        "        client.login({email: login.value, password: password.value}).done(function(r) { \r\n"
        "            log('Login success, token = ' + r.token);\r\n"
        "        }).fail(function(e) {\r\n"
        "            log('Login failed, error = ' + e.error);\r\n"
        "        })\r\n"
        "    }\r\n"
        "    btn_logout.onclick = function() {\r\n"
        "        client.deinit();\r\n"
        "    }\r\n"
        "    btn_connect.onclick = function() {\r\n"
        "        client.init({'baseUrl': server_url.value});\r\n"
        "    }\r\n"
        "</script>\r\n"
        "<pre id='log'></pre>\r\n"
        "</body>\r\n"
        "</html>\r\n";
    sample_html.close();
    std::cout << "\t> OK" << std::endl;
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::exportLibCliWebJSFile() {

    std::string sFilename = this->getExportDir() + "/dist/" + this->getPackageName() + ".js";
    std::ofstream libwjscppcli_web_js_file;
    std::cout << " * write code to " << sFilename << std::endl;
    libwjscppcli_web_js_file.open(sFilename);
    if (!libwjscppcli_web_js_file.is_open()) {
        WsjcppLog::err(TAG, "Could not open file for write '" + sFilename + "'");
        return false;
    }

    libwjscppcli_web_js_file <<
        "// This file was automatically generated by " << this->getAppName() << " (" + this->getAppVersion() + "), date: " << this->getDateNow() << "\r\n"
        "window." << this->getClassName() << " = window." << this->getClassName() << " || (function() { \r\n"
        "    var self = {};\r\n"
        "    self.appName = '" + this->getAppName() + "';\r\n"
        "    self.appVersion = '" + this->getAppVersion() + "';\r\n"
        "    self.appBuildDate = '" + this->getDateNow() + "';\r\n"
        "    var _lastm = 0;\r\n"
        "    var _listeners = {};\r\n"
        "    var _connectionState = '?';\r\n"
        "    var _tokenValue = '';\r\n"
        "    var _events = {\r\n";
    
    for (int i = 0; i < m_vEvents.size(); i++) {
        libwjscppcli_web_js_file <<
        "        '" << m_vEvents[i] << "': [],\r\n";
    }

    libwjscppcli_web_js_file <<
        "    };\r\n"
        "    function _lm() { _lastm++; return 'm' + _lastm; };\r\n"
        "    console.warn('" + this->getClassName() + " (" + this->getAppVersion() + ")');\r\n"
        "    self.promise = function() {\r\n"
        "        return {\r\n"
        "            completed: false, failed: false, successed: false, \r\n"
        "            done: function(callback) {\r\n"
        "                this.done_callback = callback;\r\n"
        "                if (this.completed && typeof this.done_callback === 'function' && this.successed) {\r\n"
        "                    this.done_callback.apply(this, this.result_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            fail: function(callback) {\r\n"
        "                this.fail_callback = callback;\r\n"
        "                if (this.completed && typeof this.fail_callback === 'function' && this.failed) {\r\n"
        "                    this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            resolve: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.result_arguments = arguments; // [];\r\n"
        "                    if (typeof this.done_callback === 'function') {\r\n"
        "                        this.done_callback.apply(this, this.result_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.successed = true;\r\n"
        "                this.completed = true;\r\n"
        "            },\r\n"
        "            reject: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.error_arguments = arguments;\r\n"
        "                    if (typeof this.fail_callback === 'function') {\r\n"
        "                        this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.failed = true;\r\n"
        "                this.completed = true;\r\n"
        "            }\r\n"
        "        }; // end of promise\r\n"
        "    };\r\n"
        "    self.waitAllPromises = function(arr_promise) {\r\n"
        "        var p = self.promise();\r\n"
        "        var max_len = arr_promise.length;\r\n"
        "        var result = [];\r\n"
        "        function cmpl(r) {\r\n"
        "            result.push(r);\r\n"
        "            if (result.length == max_len) {\r\n"
        "                p.resolve(result);\r\n"
        "            }\r\n"
        "        };\r\n"
        "        for (var i in arr_promise) {\r\n"
        "            arr_promise[i].done(cmpl).fail(cmpl);\r\n"
        "        }\r\n"
        "        return p;\r\n"
        "    };\r\n"
        "    self.setToken = function(token) {\r\n"
        "        var date = new Date( new Date().getTime() + (7 * 24 * 60 * 60 * 1000) ); // cookie on week\r\n"
        "        document.cookie = '" + this->getClassName() + "token=' + encodeURIComponent(token) + '; path=/; expires='+date.toUTCString();\r\n"
        "    }\r\n"
        "    self.removeToken = function() {\r\n"
        "        _tokenValue = '';"
        "        document.cookie = '" + this->getClassName() + "token=; path=/;';\r\n"
        "    }\r\n"
        "    self.getToken = function() {\r\n"
        "        var matches = document.cookie.match(new RegExp(\r\n"
        "            '(?:^|; )' + '" + this->getClassName() + "token'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, '\\\\$1') + '=([^;]*)'\r\n"
        "        ));\r\n"
        "        return matches ? decodeURIComponent(matches[1]) : '';\r\n"
        "    }\r\n"
        "    _tokenValue = self.getToken();\r\n"
        "    self.bind = function(name, f) { _events[name].push(f); }\r\n"
        "    self.unbind = function(name) { _events[name] = []; }\r\n"
        "    function _call(name, data) {\r\n"
        "        function __call(f, data) { setTimeout(function() { f(data) },1)}"
        "        for (var i = 0; i < _events[name].length; i++) {\r\n"
        "            __call(_events[name][i], data);\r\n"
        "        }\r\n"
        "    }\r\n"
        "    self.bind('server', function(response) { \r\n"
        "       console.warn('All: ', response);\r\n"
        "       if (response.app != self.appName) {\r\n"
        "           console.error('AppName: ' + response.app + ', but expected ' + self.appName);\r\n"
        "       }\r\n"
        "       if (response.version != self.appVersion) {\r\n"
        "           console.error('AppVersion: ' + response.version + ', but expected ' + self.appVersion);\r\n"
        "       }\r\n"
        "    }); \r\n"
        "    self.handleCommand = function(response) {\r\n"
        "       var lstn = _listeners[response.m];\r\n"
        "       if (lstn) {\r\n"
        "           setTimeout(function() {\r\n"
        "               if (response['error']) {\r\n"
        "                   lstn.reject(response);\r\n"
        "               } else {\r\n"
        "                   lstn.resolve(response);\r\n"
        "               }\r\n"
        "               delete _listeners[response.m];\r\n"
        "           },1);\r\n"
        "       } else if (_events[response.cmd]) {\r\n"
        "           _call(response.cmd, response);"
        "       } else {\r\n"
        "           console.error('Not found handler for [' + response.cmd + '/' + response.m + ']');\r\n"
        "       }\r\n"
        "   };\r\n"
        "   self.send = function(obj, def) {\r\n"
        "       obj.m = obj.m || _lm();\r\n"
        "       _listeners[obj.m] = def || self.promise();\r\n"
        "       try {\r\n"
        "           if (self.socket.readyState == 0) {\r\n"
        "               setTimeout(function() {\r\n"
        "                   self.send(obj, _listeners[obj.m]);\r\n"
        "               },1000);\r\n"
        "           } else {\r\n"
        "               self.socket.send(JSON.stringify(obj));\r\n"
        "           }\r\n"
        "       } catch(e) {\r\n"
        "           console.error(e);\r\n"
        "       }\r\n"
        "       return _listeners[obj.m];\r\n"
        "    }\r\n"
        "    self.init = function(initParams) {\r\n"
        "        if (!initParams.baseUrl) console.error('Expected baseUrl on initParams');\r\n"
        "        self.socket = new WebSocket(initParams.baseUrl);\r\n"
        "        self.socket.onopen = function() {\r\n"
        "           console.log('WS Opened');\r\n"
        "           _call('connected', {});\r\n"
        "           if (_tokenValue != '') self.token();\r\n"
        "        };\r\n"
        "        self.socket.onclose = function(event) {\r\n"
        "           console.log('Closed');\r\n"
        "           if (event.wasClean) {\r\n"
        "               _call('disconnected', {});\r\n"
        "           } else {\r\n"
        "               _call('broken', {});\r\n"
        "               setTimeout(function() {\r\n"
        "                   _call('reconnecting', {});\r\n"
        "                   self.init(initParams);\r\n"
        "               }, 10000);\r\n"
        "             // Try reconnect after 5 sec\r\n"
        "           }\r\n"
        "           console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
        "       };\r\n"
        "       self.socket.onmessage = function(event) {\r\n"
        "           // console.log('Received: ' + event.data);\r\n"
        "           try {\r\n"
        "               var response = JSON.parse(event.data);\r\n"
        "               self.handleCommand(response);\r\n"
        "           } catch(e) {\r\n"
        "               console.error(e);\r\n"
        "           }\r\n"
        "           \r\n"
        "       };\r\n"
        "       self.socket.onerror = function(error) {\r\n"
        "           console.log('Error: ' + error.message);\r\n"
        "       };\r\n"
        "    }\r\n"
        "    self.deinit = function(initParams) {\r\n"
        "       self.removeToken();\r\n"
        "       self.socket.close();\r\n"
        "    }\r\n"
        "   self.userProfile = {bInitUserProfile: false}\r\n"
        "   self.updateUserProfileAsync = function() {\r\n"
        "       setTimeout(function() {\r\n"
        "           self.user().done(function(r) {\r\n"
        "               self.userProfile.bInitUserProfile == true;\r\n"
        "               self.userProfile.university = r.data.university;\r\n"
        "               self.userProfile.country = r.data.country;\r\n"
        "               self.userProfile.city = r.data.city;\r\n"
        "               self.userinfo = {};\r\n"
        "               self.userinfo.id = r.data.id;\r\n"
        "               self.userinfo.nick = r.data.nick;\r\n"
        "               self.userinfo.email = r.data.email;\r\n"
        "               self.userinfo.role = r.data.role;\r\n"
        "               self.userinfo.logo = r.data.logo;\r\n"
        "               _call('userdata', r.data);\r\n"
        "           }).fail(function() {\r\n"
        "               self.removeToken();\r\n"
        "               _call('userdata', {});\r\n"
        "           });\r\n"
        "       },10);\r\n"
        "   }\r\n";

    std::map<std::string, WsjcppJsonRpc20HandlerBase*>::iterator it = g_pWsjcppJsonRpc20HandlerList->begin();
    for (; it!=g_pWsjcppJsonRpc20HandlerList->end(); ++it) {
        std::string sCmd = it->first;
        WsjcppJsonRpc20HandlerBase* pWsjcppJsonRpc20HandlerBase = it->second;
        libwjscppcli_web_js_file <<
            "    self." << sCmd << " = function(params) {\r\n";

        libwjscppcli_web_js_file <<
            "       // Access unauthorized: " << (pWsjcppJsonRpc20HandlerBase->haveUnauthorizedAccess() ? "yes" : "no") << "\r\n"
            "       // Access user: " << (pWsjcppJsonRpc20HandlerBase->haveUserAccess() ? "yes" : "no") << "\r\n"
            "       // Access tester: " << (pWsjcppJsonRpc20HandlerBase->haveTesterAccess() ? "yes" : "no") << "\r\n"
            "       // Access admin: " << (pWsjcppJsonRpc20HandlerBase->haveAdminAccess() ? "yes" : "no") << "\r\n";
        
        if (pWsjcppJsonRpc20HandlerBase->getActivatedFromVersion() != "") {
            libwjscppcli_web_js_file <<
                "       // Activated From Version: " << pWsjcppJsonRpc20HandlerBase->getActivatedFromVersion() << "\r\n";
        }
        
        if (pWsjcppJsonRpc20HandlerBase->getDeprecatedFromVersion() != "") {
            libwjscppcli_web_js_file <<
                "       // Deprecated From Version: " + pWsjcppJsonRpc20HandlerBase->getDeprecatedFromVersion() << "\r\n";
        }
        
        std::vector<WsjcppJsonRpc20ParamDef> vVin = pWsjcppJsonRpc20HandlerBase->getParamsDef();
        if (vVin.size() > 0) {
            libwjscppcli_web_js_file <<
                "       // Input params:\r\n"; 
        }
        for (int i = 0; i < vVin.size(); i++) {
            WsjcppJsonRpc20ParamDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            libwjscppcli_web_js_file <<
                "       // * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() + " (" + inDef.getDescription() + ") \r\n";
        }

        libwjscppcli_web_js_file <<
            "        params = params || {};\r\n"
            "        params.cmd = '" << sCmd << "';\r\n";
            // check required
        for (int i = 0; i < vVin.size(); i++) {
            WsjcppJsonRpc20ParamDef inDef = vVin[i];
            if (inDef.isRequired()) {
                std::string nameIn = std::string(vVin[i].getName());
                libwjscppcli_web_js_file <<
                    "        if (params['" + nameIn + "'] == undefined) {\r\n"
                    "             console.error('Parameter \"" << nameIn << "\" expected (lib)');\r\n"
                    "        }\r\n";
            }
        }
        if (sCmd == "login") {
            libwjscppcli_web_js_file <<
                "        var ret = self.promise()\r\n"
                "        self.send(params).done(function(r) {\r\n"
                "            _tokenValue = r.token;\r\n"
                "            console.log(_tokenValue);\r\n"
                "            self.userinfo = r.user;\r\n"
                "            self.setToken(_tokenValue);\r\n"
                "            self.updateUserProfileAsync();\r\n"
                "            ret.resolve(r);\r\n"
                "        }).fail(function(err) {\r\n"
                "            self.removeToken();\r\n"
                "            ret.reject(err);\r\n"
                "        })\r\n"
                "        return ret;\r\n";
        } else if (sCmd == "token") {
            libwjscppcli_web_js_file <<
                "         if (_tokenValue != '') {\r\n"
                "             var ret = self.promise()\r\n"
                "             params.token = _tokenValue;\r\n"
                "             self.send(params).done(function(r) {\r\n"
                "                 self.updateUserProfileAsync();\r\n"
                "                 ret.resolve(r);\r\n"
                "             }).fail(function(err) {\r\n"
                "                 self.removeToken();\r\n"
                "                 _call('userdata', {});\r\n"
                "                 ret.reject(err);\r\n"
                "             })\r\n"
                "             return ret;\r\n"
                "         } else {\r\n"
                "             return self.send(params);\r\n"
                "         }\r\n";
        } else {
            libwjscppcli_web_js_file 
                << "        return self.send(params);\r\n";
        }

        libwjscppcli_web_js_file 
            << "    }\r\n\r\n";
    }

    libwjscppcli_web_js_file <<
        "    return self;\r\n"
        "})();\r\n";

    libwjscppcli_web_js_file.close();
    std::cout << "\t> OK" << std::endl;
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ExportCliWebJs::exportLibCliWebServiceTSFile() {
    std::string sFilename = this->getExportDir() + "/dist/" + this->getPackageName() + ".service.ts";
    std::ofstream libwjscppcli_web_service_ts_file;
    std::cout << " * write code to " << sFilename << std::endl;
    libwjscppcli_web_service_ts_file.open(sFilename);
    if (!libwjscppcli_web_service_ts_file.is_open()) {
        WsjcppLog::err(TAG, "Could not open file for write '" + sFilename + "'");
        return false;
    }

    libwjscppcli_web_service_ts_file <<
        "// This file was automatically generated by " << this->getAppName() << " (v" + this->getAppVersion() + "), date: " << this->getDateNow() << "\r\n"
        "import { Injectable, EventEmitter } from '@angular/core';\r\n"
        "import { PlatformLocation } from '@angular/common';\r\n"
        "import { ToastrService } from 'ngx-toastr';\r\n"
        "\r\n"
        "declare var FreeHackQuestClientConfig: any;\r\n"
        "\r\n"
        "@Injectable({providedIn: 'root'})\r\n"
        "export class " + this->getClassName() + " {\r\n"
        "  private appName: string = '" + this->getAppName() + "';\r\n"
        "  private appVersion: string = '" + this->getAppVersion() + "';\r\n"
        "  private appBuildDate: string = '" + this->getDateNow() + "';\r\n"
        "  isAuthorized: boolean = false;\r\n"
        "  connectionState: string = '';\r\n"
        "  serverHost: string = 'localhost';\r\n"
        "  currentProtocol: string = 'http:';\r\n"
        "  private _tokenName: string = '" + this->getClassName() + "_token';\r\n"
        "  userdata: any = {};\r\n"
        "  changedState = new EventEmitter<any>();\r\n"
        "  private _lastm: number = 0;\r\n"
        "  private _listeners: any = {};\r\n"
        "  private _connectionState: string = '?';\r\n"
        "  private _tokenValue: string = '';\r\n"
        "  private _socket: WebSocket = null;\r\n"
        "  private _events: any = {\r\n";

    for (int i = 0; i < m_vEvents.size(); i++) {
        libwjscppcli_web_service_ts_file <<
        "    '" << m_vEvents[i] << "': [],\r\n";
    }

    libwjscppcli_web_service_ts_file <<
        "  };\r\n"
        "  private _lastConnectionParams: any = null;\r\n"
        "\r\n"
        "  constructor(\r\n"
        "    private _location: PlatformLocation,\r\n"
        "  ) {\r\n"
        "    this.serverHost = this._location.hostname;\r\n"
        "    this.currentProtocol = this._location.protocol;\r\n"
        "    console.warn('" + this->getClassName() + " (" + this->getAppVersion() + ")');\r\n"
        "    this._tokenValue = this.getToken();\r\n"
        "  }\r\n"
        "\r\n"
        "  setToken(token: string) {\r\n"
        "    var date = new Date( new Date().getTime() + (7 * 24 * 60 * 60 * 1000) ); // cookie on week\r\n"
        "    document.cookie = this._tokenName + '=' + encodeURIComponent(token) + '; path=/; expires='+date.toUTCString();\r\n"
        "  }\r\n"
        "  \r\n"
        "  removeToken() {\r\n"
        "    this._tokenValue = '';\r\n"
        "    document.cookie = this._tokenName + '=; path=/;';\r\n"
        "  }\r\n"
        "\r\n"
        "  getToken() {\r\n"
        "    var matches = document.cookie.match(new RegExp(\r\n"
        "      '(?:^|; )' + this._tokenName.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, '\\\\$1') + '=([^;]*)'\r\n"
        "    ));\r\n"
        "    return matches ? decodeURIComponent(matches[1]) : '';\r\n"
        "  }\r\n"
        "\r\n"
        "  bind(name: string, f: Function) {\r\n"
        "    this._events[name].push(f);\r\n"
        "  }\r\n"
        "\r\n"
        "  unbind(name: string) {\r\n"
        "    this._events[name] = [];\r\n"
        "  }\r\n"
        "\r\n"
        "  _call(name: string, data: any) {\r\n"
        "    function __call(f: any, data: any) {\r\n"
        "      setTimeout(function() { f(data) },1)\r\n"
        "    }\r\n"
        "    for (var i = 0; i < this._events[name].length; i++) {\r\n"
        "      __call(this._events[name][i], data);\r\n"
        "    }\r\n"
        "  }\r\n"
        "\r\n"
        "  private socketOnOpen() {\r\n"
        "    console.log('FreeHackQuestClient WS Opened');\r\n"
        "    this._call('connected', {});\r\n"
        "    if (this._tokenValue != '') this.token();\r\n"
        "  }\r\n"
        "\r\n"
        "  private socketOnClose(event: any) {\r\n"
        "    console.log('FreeHackQuestClient Closed');\r\n"
        "    if (event.wasClean) {\r\n"
        "        this._call('disconnected', {});\r\n"
        "    } else {\r\n"
        "        this._call('broken', {});\r\n"
        "        setTimeout(function() {\r\n"
        "            this._call('reconnecting', {});\r\n"
        "            if (this._lastConnectionParams != null) {\r\n"
        "                this.connectToServer(this._lastConnectionParams);\r\n"
        "            }\r\n"
        "        }, 10000);\r\n"
        "        // Try reconnect after 10 sec\r\n"
        "    }\r\n"
        "    console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
        "  }\r\n"
        "\r\n"
        "  private _socketOnMessage(event: any) {\r\n"
        "    console.log('FreeHackQuestClient Received: ' + event.data);\r\n"
        "    try {\r\n"
        "        var response = JSON.parse(event.data);\r\n"
        "        // this._handleCommand(response);\r\n"
        "    } catch(e) {\r\n"
        "        console.error(e);\r\n"
        "    }\r\n"
        "  }\r\n"
        "\r\n"
        "  private _socketOnError(error: any) {\r\n"
        "    console.log('FreeHackQuestClient Socket Error: ', error);\r\n"
        "  }\r\n"
        "\r\n"
        "  connectToServer(initParams: any) {\r\n"
        "    this._lastConnectionParams = null;\r\n"
        "    if (!initParams.baseUrl) console.error('Expected baseUrl on initParams');\r\n"
        "    this._lastConnectionParams = initParams;\r\n"
        "    this._socket = new WebSocket(initParams.baseUrl);\r\n"
        "    this._socket.onopen = (() => this.socketOnOpen());\r\n"
        "    this._socket.onclose = ((event: any) => this.socketOnClose(event));\r\n"
        "    this._socket.onmessage = ((event: any) => this._socketOnMessage(event));\r\n"
        "    this._socket.onerror = ((error: any) => this._socketOnError(error));\r\n"
        "  }\r\n"
        "\r\n"
        "  private _lm() { this._lastm++; return 'm' + this._lastm; };\r\n"
        "\r\n";


    libwjscppcli_web_service_ts_file <<
        "    };\r\n"
        "    function _lm() { _lastm++; return 'm' + _lastm; };\r\n"
        "    self.promise = function() {\r\n"
        "        return {\r\n"
        "            completed: false, failed: false, successed: false, \r\n"
        "            done: function(callback) {\r\n"
        "                this.done_callback = callback;\r\n"
        "                if (this.completed && typeof this.done_callback === 'function' && this.successed) {\r\n"
        "                    this.done_callback.apply(this, this.result_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            fail: function(callback) {\r\n"
        "                this.fail_callback = callback;\r\n"
        "                if (this.completed && typeof this.fail_callback === 'function' && this.failed) {\r\n"
        "                    this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            resolve: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.result_arguments = arguments; // [];\r\n"
        "                    if (typeof this.done_callback === 'function') {\r\n"
        "                        this.done_callback.apply(this, this.result_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.successed = true;\r\n"
        "                this.completed = true;\r\n"
        "            },\r\n"
        "            reject: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.error_arguments = arguments;\r\n"
        "                    if (typeof this.fail_callback === 'function') {\r\n"
        "                        this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.failed = true;\r\n"
        "                this.completed = true;\r\n"
        "            }\r\n"
        "        }; // end of promise\r\n"
        "    };\r\n"
        "    self.waitAllPromises = function(arr_promise) {\r\n"
        "        var p = self.promise();\r\n"
        "        var max_len = arr_promise.length;\r\n"
        "        var result = [];\r\n"
        "        function cmpl(r) {\r\n"
        "            result.push(r);\r\n"
        "            if (result.length == max_len) {\r\n"
        "                p.resolve(result);\r\n"
        "            }\r\n"
        "        };\r\n"
        "        for (var i in arr_promise) {\r\n"
        "            arr_promise[i].done(cmpl).fail(cmpl);\r\n"
        "        }\r\n"
        "        return p;\r\n"
        "    };\r\n"
        "    self.setToken = function(token) {\r\n"
        "        var date = new Date( new Date().getTime() + (7 * 24 * 60 * 60 * 1000) ); // cookie on week\r\n"
        "        document.cookie = '" + this->getClassName() + "token=' + encodeURIComponent(token) + '; path=/; expires='+date.toUTCString();\r\n"
        "    }\r\n"
        "    self.removeToken = function() {\r\n"
        "        _tokenValue = '';"
        "        document.cookie = '" + this->getClassName() + "token=; path=/;';\r\n"
        "    }\r\n"
        "    self.getToken = function() {\r\n"
        "        var matches = document.cookie.match(new RegExp(\r\n"
        "            '(?:^|; )' + '" + this->getClassName() + "token'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, '\\\\$1') + '=([^;]*)'\r\n"
        "        ));\r\n"
        "        return matches ? decodeURIComponent(matches[1]) : '';\r\n"
        "    }\r\n"
        "    _tokenValue = self.getToken();\r\n"
        "    self.bind = function(name, f) { _events[name].push(f); }\r\n"
        "    self.unbind = function(name) { _events[name] = []; }\r\n"
        "    function _call(name, data) {\r\n"
        "        function __call(f, data) { setTimeout(function() { f(data) },1)}"
        "        for (var i = 0; i < _events[name].length; i++) {\r\n"
        "            __call(_events[name][i], data);\r\n"
        "        }\r\n"
        "    }\r\n"
        "    self.bind('server', function(response) { \r\n"
        "       console.warn('All: ', response);\r\n"
        "       if (response.app != self.appName) {\r\n"
        "           console.error('AppName: ' + response.app + ', but expected ' + self.appName);\r\n"
        "       }\r\n"
        "       if (response.version != self.appVersion) {\r\n"
        "           console.error('AppVersion: ' + response.version + ', but expected ' + self.appVersion);\r\n"
        "       }\r\n"
        "    }); \r\n"
        "    self.handleCommand = function(response) {\r\n"
        "       var lstn = _listeners[response.m];\r\n"
        "       if (lstn) {\r\n"
        "           setTimeout(function() {\r\n"
        "               if (response['error']) {\r\n"
        "                   lstn.reject(response);\r\n"
        "               } else {\r\n"
        "                   lstn.resolve(response);\r\n"
        "               }\r\n"
        "               delete _listeners[response.m];\r\n"
        "           },1);\r\n"
        "       } else if (_events[response.cmd]) {\r\n"
        "           _call(response.cmd, response);"
        "       } else {\r\n"
        "           console.error('Not found handler for [' + response.cmd + '/' + response.m + ']');\r\n"
        "       }\r\n"
        "   };\r\n"
        "   self.send = function(obj, def) {\r\n"
        "       obj.m = obj.m || _lm();\r\n"
        "       _listeners[obj.m] = def || self.promise();\r\n"
        "       try {\r\n"
        "           if (self.socket.readyState == 0) {\r\n"
        "               setTimeout(function() {\r\n"
        "                   self.send(obj, _listeners[obj.m]);\r\n"
        "               },1000);\r\n"
        "           } else {\r\n"
        "               self.socket.send(JSON.stringify(obj));\r\n"
        "           }\r\n"
        "       } catch(e) {\r\n"
        "           console.error(e);\r\n"
        "       }\r\n"
        "       return _listeners[obj.m];\r\n"
        "    }\r\n"
        "    self.init = function(initParams) {\r\n"
        "        if (!initParams.baseUrl) console.error('Expected baseUrl on initParams');\r\n"
        "        self.socket = new WebSocket(initParams.baseUrl);\r\n"
        "        self.socket.onopen = function() {\r\n"
        "           console.log('WS Opened');\r\n"
        "           _call('connected', {});\r\n"
        "           if (_tokenValue != '') self.token();\r\n"
        "        };\r\n"
        "        self.socket.onclose = function(event) {\r\n"
        "           console.log('Closed');\r\n"
        "           if (event.wasClean) {\r\n"
        "               _call('disconnected', {});\r\n"
        "           } else {\r\n"
        "               _call('broken', {});\r\n"
        "               setTimeout(function() {\r\n"
        "                   _call('reconnecting', {});\r\n"
        "                   self.init(initParams);\r\n"
        "               }, 10000);\r\n"
        "             // Try reconnect after 5 sec\r\n"
        "           }\r\n"
        "           console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
        "       };\r\n"
        "       self.socket.onmessage = function(event) {\r\n"
        "           // console.log('Received: ' + event.data);\r\n"
        "           try {\r\n"
        "               var response = JSON.parse(event.data);\r\n"
        "               self.handleCommand(response);\r\n"
        "           } catch(e) {\r\n"
        "               console.error(e);\r\n"
        "           }\r\n"
        "           \r\n"
        "       };\r\n"
        "       self.socket.onerror = function(error) {\r\n"
        "           console.log('Error: ' + error.message);\r\n"
        "       };\r\n"
        "    }\r\n"
        "    self.deinit = function(initParams) {\r\n"
        "       self.removeToken();\r\n"
        "       self.socket.close();\r\n"
        "    }\r\n"
        "   self.userProfile = {bInitUserProfile: false}\r\n"
        "   self.updateUserProfileAsync = function() {\r\n"
        "       setTimeout(function() {\r\n"
        "           self.user().done(function(r) {\r\n"
        "               self.userProfile.bInitUserProfile == true;\r\n"
        "               self.userProfile.university = r.data.university;\r\n"
        "               self.userProfile.country = r.data.country;\r\n"
        "               self.userProfile.city = r.data.city;\r\n"
        "               self.userinfo = {};\r\n"
        "               self.userinfo.id = r.data.id;\r\n"
        "               self.userinfo.nick = r.data.nick;\r\n"
        "               self.userinfo.email = r.data.email;\r\n"
        "               self.userinfo.role = r.data.role;\r\n"
        "               self.userinfo.logo = r.data.logo;\r\n"
        "               _call('userdata', r.data);\r\n"
        "           }).fail(function() {\r\n"
        "               self.removeToken();\r\n"
        "               _call('userdata', {});\r\n"
        "           });\r\n"
        "       },10);\r\n"
        "   }\r\n";

    std::map<std::string, WsjcppJsonRpc20HandlerBase*>::iterator it = g_pWsjcppJsonRpc20HandlerList->begin();
    for (; it!=g_pWsjcppJsonRpc20HandlerList->end(); ++it) {
        std::string sCmd = it->first;
        WsjcppJsonRpc20HandlerBase* pWsjcppJsonRpc20HandlerBase = it->second;
        libwjscppcli_web_service_ts_file <<
            "    self." << sCmd << " = function(params) {\r\n";

        libwjscppcli_web_service_ts_file <<
            "       // Access unauthorized: " << (pWsjcppJsonRpc20HandlerBase->haveUnauthorizedAccess() ? "yes" : "no") << "\r\n"
            "       // Access user: " << (pWsjcppJsonRpc20HandlerBase->haveUserAccess() ? "yes" : "no") << "\r\n"
            "       // Access tester: " << (pWsjcppJsonRpc20HandlerBase->haveTesterAccess() ? "yes" : "no") << "\r\n"
            "       // Access admin: " << (pWsjcppJsonRpc20HandlerBase->haveAdminAccess() ? "yes" : "no") << "\r\n";
        
        if (pWsjcppJsonRpc20HandlerBase->getActivatedFromVersion() != "") {
            libwjscppcli_web_service_ts_file <<
                "       // Activated From Version: " << pWsjcppJsonRpc20HandlerBase->getActivatedFromVersion() << "\r\n";
        }
        
        if (pWsjcppJsonRpc20HandlerBase->getDeprecatedFromVersion() != "") {
            libwjscppcli_web_service_ts_file <<
                "       // Deprecated From Version: " + pWsjcppJsonRpc20HandlerBase->getDeprecatedFromVersion() << "\r\n";
        }
        
        std::vector<WsjcppJsonRpc20ParamDef> vVin = pWsjcppJsonRpc20HandlerBase->getParamsDef();
        if (vVin.size() > 0) {
            libwjscppcli_web_service_ts_file <<
                "       // Input params:\r\n"; 
        }
        for (int i = 0; i < vVin.size(); i++) {
            WsjcppJsonRpc20ParamDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            libwjscppcli_web_service_ts_file <<
                "       // * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() + " (" + inDef.getDescription() + ") \r\n";
        }

        libwjscppcli_web_service_ts_file <<
            "        params = params || {};\r\n"
            "        params.cmd = '" << sCmd << "';\r\n";
            // check required
        for (int i = 0; i < vVin.size(); i++) {
            WsjcppJsonRpc20ParamDef inDef = vVin[i];
            if (inDef.isRequired()) {
                std::string nameIn = std::string(vVin[i].getName());
                libwjscppcli_web_service_ts_file <<
                    "        if (params['" + nameIn + "'] == undefined) {\r\n"
                    "             console.error('Parameter \"" << nameIn << "\" expected (lib)');\r\n"
                    "        }\r\n";
            }
        }
        if (sCmd == "login") {
            libwjscppcli_web_service_ts_file <<
                "        var ret = self.promise()\r\n"
                "        self.send(params).done(function(r) {\r\n"
                "            _tokenValue = r.token;\r\n"
                "            console.log(_tokenValue);\r\n"
                "            self.userinfo = r.user;\r\n"
                "            self.setToken(_tokenValue);\r\n"
                "            self.updateUserProfileAsync();\r\n"
                "            ret.resolve(r);\r\n"
                "        }).fail(function(err) {\r\n"
                "            self.removeToken();\r\n"
                "            ret.reject(err);\r\n"
                "        })\r\n"
                "        return ret;\r\n";
        } else if (sCmd == "token") {
            libwjscppcli_web_service_ts_file <<
                "         if (_tokenValue != '') {\r\n"
                "             var ret = self.promise()\r\n"
                "             params.token = _tokenValue;\r\n"
                "             self.send(params).done(function(r) {\r\n"
                "                 self.updateUserProfileAsync();\r\n"
                "                 ret.resolve(r);\r\n"
                "             }).fail(function(err) {\r\n"
                "                 self.removeToken();\r\n"
                "                 _call('userdata', {});\r\n"
                "                 ret.reject(err);\r\n"
                "             })\r\n"
                "             return ret;\r\n"
                "         } else {\r\n"
                "             return self.send(params);\r\n"
                "         }\r\n";
        } else {
            libwjscppcli_web_service_ts_file 
                << "         return self.send(params);\r\n";
        }

        libwjscppcli_web_service_ts_file 
            << "    }\r\n\r\n";
    }

    libwjscppcli_web_service_ts_file <<
        "    return self;\r\n"
        "})();\r\n";

    libwjscppcli_web_service_ts_file.close();
    std::cout << "\t> OK" << std::endl;
    return true;
}