#include "wsjcpp_jsonrpc20_export_cli_base.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <sstream>

// ---------------------------------------------------------------------
// WsjcppJsonRpc20ExportCliBase

WsjcppJsonRpc20ExportCliBase::WsjcppJsonRpc20ExportCliBase(
    const std::string &sExportDir,
    const std::string &sPackageName
) {
    TAG = "WsjcppJsonRpc20ExportCliBase";
    m_sExportDir = WsjcppCore::doNormalizePath(sExportDir);
    m_sPackageName = sPackageName;
    m_sAppName = "unknown";
    m_sAppVersion = "unknown";
    m_sAuthorName = "Unknown";
    m_sAuthorEmail = "unknown";
    m_sClassName = "Unknown";
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getExportDir() const {
    return m_sExportDir;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getPackageName() const {
    return m_sPackageName;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliBase::setAppName(const std::string &sAppName) {
    m_sAppName = sAppName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getAppName() const {
    return m_sAppName;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliBase::setAppVersion(const std::string &sAppVersion) {
    // https://www.python.org/dev/peps/pep-0440/
    // [N!]N(.N)*[{a|b|rc}N][.postN][.devN]
    // TODO regexp 
    m_sAppVersion = sAppVersion;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getAppVersion() const {
    return m_sAppVersion;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliBase::setAuthorName(const std::string &sAuthorName) {
    m_sAuthorName = sAuthorName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getAuthorName() const {
    return m_sAuthorName;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliBase::setAuthorEmail(const std::string &sAuthorEmail) {
    m_sAuthorEmail = sAuthorEmail;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getAuthorEmail() const {
    return m_sAuthorEmail;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20ExportCliBase::setClassName(const std::string &sClassName) {
    m_sClassName = sClassName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20ExportCliBase::getClassName() const {
    return m_sClassName;
}

// ---------------------------------------------------------------------