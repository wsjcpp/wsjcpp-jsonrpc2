#include "wsjcpp_jsonrpc20.h"
#include <wsjcpp_core.h>
#include <regex>

// ---------------------------------------------------------------------
// WsjcppJsonRpc20Error - 

WsjcppJsonRpc20Error::WsjcppJsonRpc20Error(
    int nErrorCode,
    const std::string &sErrorMessage,
    const std::vector<std::pair<std::string,std::string>> &vErrorContext
) {
    m_nErrorCode = nErrorCode;
    m_sErrorMessage = sErrorMessage;
    m_vErrorContext = vErrorContext;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20Error::getErrorCode() const {
    return m_nErrorCode;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Error::getErrorMessage() const {
    return m_sErrorMessage;
}

// ---------------------------------------------------------------------

const std::vector<std::pair<std::string,std::string>> &WsjcppJsonRpc20Error::getErrorContext() const {
    return m_vErrorContext;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20Error::toJson() {
    nlohmann::json jsonRet;
    jsonRet["code"] = m_nErrorCode;
    jsonRet["message"] = m_sErrorMessage;
    if (m_vErrorContext.size() > 0) {
        jsonRet["context"] = nlohmann::json();
        for (int i = 0; i < m_vErrorContext.size(); i++) {
            jsonRet["context"][m_vErrorContext[i].first] = m_vErrorContext[i].second;
        }
    }
    return jsonRet;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20UserSession - all data by current user session

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession() {
    TAG = "WsjcppJsonRpc20UserSession";
    m_nUserID = 0;
    m_sUserRole = "";
    m_sUserEmail = "";
    m_sUserUuid = "";
    m_sUserName = "";
    m_sSessionUuid = "";
    m_nSessionCreated = 0;
    m_nSessionUpdated = 0;
    m_nSessionExpireAt = 0;
    m_jsonSessionCustom = nlohmann::json{};
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession::WsjcppJsonRpc20UserSession(nlohmann::json const& obj) : WsjcppJsonRpc20UserSession() {
    this->fillFrom(obj);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::fillFrom(const nlohmann::json &obj) {
    if (obj.find("uuid") != obj.end()) {
        try {
            m_sSessionUuid = obj.at("uuid").get<std::string>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.uuid in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_sSessionUuid = "";
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'uuid' in struct");
        m_sSessionUuid = "";
    }

    if (obj.find("created") != obj.end()) {
        try {
            m_nSessionCreated = obj.at("created").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.created in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionCreated = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'created' in struct");
        m_nSessionCreated = 0;
    }

    if (obj.find("updated") != obj.end()) {
        try {
            m_nSessionUpdated = obj.at("updated").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.updated in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionUpdated = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'updated' in struct");
        m_nSessionUpdated = 0;
    }

    if (obj.find("expire_at") != obj.end()) {
        try {
            m_nSessionExpireAt = obj.at("expire_at").get<long>();
        } catch (const std::exception &e) {
            WsjcppLog::err(TAG, "Something wrong param session.expire_at in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
            m_nSessionExpireAt = 0;
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'expire_at' in struct");
        m_nSessionExpireAt = 0;
    }

    if (obj.find("custom") != obj.end()) {
        m_jsonSessionCustom = obj["custom"];
    } else {
        WsjcppLog::warn(TAG, "Not found param 'custom' in struct");
        m_jsonSessionCustom = nlohmann::json{};
    }

    if (obj.find("user") != obj.end()) {
        nlohmann::json user = obj.at("user");

        if (user.find("role") != user.end()) {
            try {
                m_sUserRole = user.at("role").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.role in struct: "
                 + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserRole = "";
            }
        } else {
            m_sUserRole = "";
        }

        if (user.find("id") != user.end()) {
            try {
                m_nUserID = user.at("id").get<int>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.id in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_nUserID = 0;
            }
        } else {
            m_nUserID = 0;
        }
        
        // user.email
        if (user.find("email") != user.end()) {
            try {
                m_sUserEmail = user.at("email").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.email in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserEmail = "";
            }
        } else {
            m_sUserEmail = "";
        }

        // user.nick
        if (user.find("name") != user.end()) {
            try {
                m_sUserName = user.at("name").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.name in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserName = "";
            }
        } else {
            m_sUserName = "";
        }

        // user.uuid
        if (user.find("uuid") != user.end()) {
            try {
                m_sUserUuid = user.at("uuid").get<std::string>();
            } catch (const std::exception &e) {
                WsjcppLog::err(TAG, "Something wrong param session.user.uuid in struct: "
                    + obj.dump() + "; error: " + std::string(e.what()));
                m_sUserUuid = "";
            }
        } else {
            m_sUserUuid = "";
        }
    } else {
        WsjcppLog::warn(TAG, "Not found param 'user' in struct");
        m_nUserID = 0;
        m_sUserRole = "";
        m_sUserName = "";
        m_sUserUuid = "";
        m_sUserEmail = "";
    }
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20UserSession::toJson() {
    nlohmann::json jsonRet;
    jsonRet["uuid"] = m_sSessionUuid;
    jsonRet["created"] = m_nSessionCreated;
    jsonRet["updated"] = m_nSessionUpdated;
    jsonRet["expire_at"] = m_nSessionExpireAt;

    nlohmann::json jsonUser;
    jsonUser["id"] = m_nUserID;
    jsonUser["uuid"] = m_sUserUuid;
    jsonUser["name"] = m_sUserName;
    jsonUser["email"] = m_sUserEmail;
    jsonUser["role"] = m_sUserRole;
    jsonRet["user"] = jsonUser;

    jsonRet["custom"] = m_jsonSessionCustom;

    return jsonRet;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getSessionUuid() {
    return m_sSessionUuid;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionUuid(const std::string& sSessionUuid) {
    m_sSessionUuid = sSessionUuid;
}


// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionCreated() {
    return m_nSessionCreated;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionCreated(long nSessionCreated) {
    m_nSessionCreated = nSessionCreated;
}

// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionUpdated() {
    return m_nSessionUpdated;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionUpdated(long nSessionUpdated) {
    m_nSessionUpdated = nSessionUpdated;
}

// ---------------------------------------------------------------------

long WsjcppJsonRpc20UserSession::getSessionExpireAt() {
    return m_nSessionExpireAt;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionExpireAt(long nSessionExpireAt) {
    m_nSessionExpireAt = nSessionExpireAt;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20UserSession::getUserId() {
    return m_nUserID;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserId(int nUserId) {
    m_nUserID = nUserId;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserUuid() {
    return m_sUserUuid;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserUuid(const std::string& sUserUuid) {
    m_sUserUuid = sUserUuid;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserName() {
    return m_sUserName;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserName(const std::string &sUserName) {
    m_sUserName = sUserName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserEmail() {
    return m_sUserEmail;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserEmail(const std::string &sUserEmail) {
    m_sUserEmail = sUserEmail;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20UserSession::getUserRole() {
    return m_sUserRole;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setUserRole(const std::string &sUserRole) {
    m_sUserRole = sUserRole;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isAdmin() {
    return m_sUserRole == "admin";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isUser() {
    return m_sUserRole == "user";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::isTester() {
    return m_sUserRole == "tester";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20UserSession::hasRole() {
    return m_sUserRole != "";
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20UserSession::getSessionCustom() {
    return m_jsonSessionCustom;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20UserSession::setSessionCustom(const nlohmann::json &jsonSessionCustom) {
    m_jsonSessionCustom = jsonSessionCustom;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20WebSocketClient

WsjcppJsonRpc20WebSocketClient::WsjcppJsonRpc20WebSocketClient() {
    TAG = "WsjcppJsonRpc20WebSocketClient";
    m_pUserSession = nullptr;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketClient::setUserSession(WsjcppJsonRpc20UserSession *pUserSession) {
    m_pUserSession = pUserSession;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession *WsjcppJsonRpc20WebSocketClient::getUserSession() {
    return m_pUserSession;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketClient::unsetUserSession() {
    m_pUserSession = nullptr;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20WebSocketServer

WsjcppJsonRpc20WebSocketServer::WsjcppJsonRpc20WebSocketServer() {
    TAG = "WsjcppJsonRpc20WebSocketServer";
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketServer::onWebSocketConnected(void *pClient, WsjcppJsonRpc20WebSocketClient *pWebSocketClient) {
    m_mapClients[pClient] = pWebSocketClient;
    WsjcppLog::info(TAG, "NewConnection url=" + pWebSocketClient->getRequestUrl()  + " "
        + pWebSocketClient->getPeerIpAddress()
        + ":" + std::to_string(pWebSocketClient->getPeerPort()));
    
    nlohmann::json jsonResult;
    jsonResult["method"] = "notify";
    jsonResult["id"] = "n0";
    nlohmann::json jsonServer;
    jsonServer["app_name"] = WSJCPP_APP_NAME;
    jsonServer["app_version"] = WSJCPP_APP_VERSION;
    jsonResult["result"] = jsonServer;
    pWebSocketClient->sendTextMessage(jsonResult.dump());
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketServer::onWebSocketDisconnected(void *pClient) {
    // TODO fix memoryleak clean usersession if need - smart pointer
    
    std::map<void *, WsjcppJsonRpc20WebSocketClient *>::iterator it;
    it = m_mapClients.find(pClient);
    m_mapClients.erase(it);
    delete m_mapClients[pClient];
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20WebSocketServer::getConnectedClients() {
    return m_mapClients.size();
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20WebSocketClient *WsjcppJsonRpc20WebSocketServer::findWebSocketClient(void *pClient) {
    WsjcppJsonRpc20WebSocketClient *pRet = nullptr;
    std::map<void *, WsjcppJsonRpc20WebSocketClient *>::iterator it;
    it = m_mapClients.find(pClient);
    if (it != m_mapClients.end()) {
        pRet = it->second;
    }
    return pRet;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketServer::sendMessageToAll(const nlohmann::json& jsonMessage) {
    std::string sTextMessage = jsonMessage.dump();
    std::map<void *, WsjcppJsonRpc20WebSocketClient *>::iterator it;
    for (it = m_mapClients.begin(); it != m_mapClients.end(); ++it) {
        it->second->sendTextMessage(sTextMessage);
    }
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20WebSocketServer::sendMessageToOne(WsjcppJsonRpc20WebSocketClient *pClient, const nlohmann::json &jsonMessage) {
    std::string sTextMessage = jsonMessage.dump();
    pClient->sendTextMessage(sTextMessage);
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20ParamDef

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef() {
    TAG = "WsjcppJsonRpc20ParamDef";
    m_bRequired = true;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef::WsjcppJsonRpc20ParamDef(const std::string &sName, const std::string &sDescription) 
    : WsjcppJsonRpc20ParamDef()
{
    m_sName = sName;
    m_sDescription = sDescription;
    
    if (sName.length() < 1) {
        WsjcppLog::throw_err(TAG, "Name must be define");
    }
    for (int i = 0; i < sName.length(); i++) {
        char c =  sName[i];
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '_') {
            std::string sError = "Not allowed character in name of param: '";
            sError += c;
            sError += "', character must be in range 'a-z0-9_'";
            WsjcppLog::throw_err(TAG, sError);
        }
        
    }
}


// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::optional() {
    m_bRequired = false;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::required() {
    m_bRequired = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::string_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'string'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_STRING);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::integer_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'integer'");
    }
    m_sType = JSONRPC20_PARAM_DEF_TYPE_INTEGER;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::bool_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'bool'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_BOOL);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::json_() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Type already defined like '" + m_sType + "', but you want change this to 'json'");
    }
    m_sType = std::string(JSONRPC20_PARAM_DEF_TYPE_JSON);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef & WsjcppJsonRpc20ParamDef::description(const std::string& s) {
    m_sDescription = s;
    return *this;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20ParamDef::toJson() {
    nlohmann::json obj;
    obj["name"] = m_sName;
    obj["type"] = m_sType;
    obj["restrict"] = m_bRequired ? "required" : "optional";
    obj["description"] = m_sDescription;
    // TODO validator description
    return obj;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getType() {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getType() const {
    return m_sType;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getName() const {
    return m_sName;
}

// ---------------------------------------------------------------------

const std::string WsjcppJsonRpc20ParamDef::getRestrict() const {
    return m_bRequired ? "required" : "optional";
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getDescription() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::string &WsjcppJsonRpc20ParamDef::getDescription() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isRequired() {
    return m_bRequired;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isOptional() {
    return !m_bRequired;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isInteger() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isString() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isBool() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20ParamDef::isJson() {
    return m_sType == JSONRPC20_PARAM_DEF_TYPE_JSON;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorStringBase *pValidatorString) {
    if (!isString()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected string data type");
    }
    m_vValidatorsString.push_back(pValidatorString);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorIntegerBase *pValidatorInteger) {
    if (!isInteger()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected integer data type");
    }
    m_vValidatorsInteger.push_back(pValidatorInteger);
    return *this;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20ParamDef::addValidator(WsjcppValidatorJsonBase *pValidatorJson) {
    if (!isJson()) {
        WsjcppLog::throw_err(TAG, "addValidator(), expected json data type");
    }
    m_vValidatorsJson.push_back(pValidatorJson);
    return *this;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorStringBase *> &WsjcppJsonRpc20ParamDef::listOfStringValidators() {
    return m_vValidatorsString;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorIntegerBase *> &WsjcppJsonRpc20ParamDef::listOfIntegerValidators() {
    return m_vValidatorsInteger;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppValidatorJsonBase *> &WsjcppJsonRpc20ParamDef::listOfJsonValidators() {
    return m_vValidatorsJson;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20Request

WsjcppJsonRpc20Request::WsjcppJsonRpc20Request(
    WsjcppJsonRpc20WebSocketClient *pClient,
    WsjcppJsonRpc20WebSocketServer *pWebSocketServer
) {
    TAG = "WsjcppJsonRpc20Request";
    m_bResponseSend = false;
    m_pWebSocketClient = pClient;
    m_pServer = pWebSocketServer;
    m_sMethodName = "unknown_method";
    m_sId = "unknown_id";
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::parseIncomeData(const std::string &sIncomeData) {
    if (!nlohmann::json::accept(sIncomeData)) {
        this->fail(WsjcppJsonRpc20Error(400, "WRONG_JSON"));
        return false;
    }

    m_jsonRequest = nlohmann::json::parse(sIncomeData);

    if (m_jsonRequest["method"].is_string()) {
        m_sMethodName = m_jsonRequest["method"];
    }

    if (m_jsonRequest["id"].is_string()) {
        m_sId = m_jsonRequest["id"];
    }

    if (m_sMethodName == "unknown_method") {
        this->fail(WsjcppJsonRpc20Error(400, "NOT_FOUND_METHOD_IN_REQUEST"));
        return false;
    }

    if (m_sId == "unknown_id") {
        this->fail(WsjcppJsonRpc20Error(400, "NOT_FOUND_ID_IN_REQUEST"));
        return false;
    }

    if (m_jsonRequest.find("jsonrpc") == m_jsonRequest.end()) {
        this->fail(WsjcppJsonRpc20Error(400, "NOT_FOUND_FIELD_JSONRPC_IN_REQUEST"));
        return false;
    } else {
        if (!m_jsonRequest["jsonrpc"].is_string()) {
            this->fail(WsjcppJsonRpc20Error(400, "FIELD_JSONRPC_EXPECTED_AS_STRING_IN_REQUEST"));
            return false;
        } else {
            std::string sJsonRpc = m_jsonRequest["jsonrpc"];
            if (sJsonRpc != "2.0") {
                this->fail(WsjcppJsonRpc20Error(400, "FIELD_JSONRPC_EXPECTED_2_DOT_0_IN_REQUEST"));
                return false;
            }
        }
    }

    if (m_jsonRequest.find("params") != m_jsonRequest.end()) {
        // std::cout << m_jsonRequest.dump() << std::endl;
        m_jsonParams = m_jsonRequest["params"];
        if (!m_jsonParams.is_object()) {
            this->fail(WsjcppJsonRpc20Error(400, "FIELD_PARAMS_EXPECTED_AS_OBJECT_IN_REQUEST"));
            return false;
        }
    }

    // m_pWsjcppJsonRpc20UserSession = m_pServer->findUserSession(m_pClient);

    WsjcppLog::info(TAG, "[WS] >>> " + m_sMethodName + ":" + m_sId);

    return true;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20WebSocketClient *WsjcppJsonRpc20Request::getWebSocketClient() {
    return m_pWebSocketClient;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20WebSocketServer *WsjcppJsonRpc20Request::getServer() {
    return m_pServer;
}

// ---------------------------------------------------------------------

// TODO deprecated
const nlohmann::json& WsjcppJsonRpc20Request::jsonRequest() {
    return m_jsonRequest;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::hasInputParam(const std::string &sParamName) {
    if (m_jsonRequest.find(sParamName) != m_jsonRequest.end()) {
        return true;
    } 
    return false;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getInputString(const std::string &sParamName, const std::string &sDefaultValue) {
    // TODO check by input defs
    std::string sRet = sDefaultValue;
    if (m_jsonRequest[sParamName].is_string()) {
        sRet = m_jsonRequest[sParamName];
    }
    return sRet;
}

// ---------------------------------------------------------------------

int WsjcppJsonRpc20Request::getInputInteger(const std::string &sParamName, int nDefaultValue) {
    // TODO check by input defs
    int nRet = nDefaultValue;
    if (m_jsonRequest[sParamName].is_number_integer()) {
        nRet = m_jsonRequest[sParamName];
    }
    return nRet;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::getInputBoolean(const std::string &sParamName, bool defaultValue) {
    // TODO check by input defs
    bool bRet = defaultValue;
    if (m_jsonRequest[sParamName].is_boolean()) {
        bRet = m_jsonRequest[sParamName];
    }
    return bRet;
}

// ---------------------------------------------------------------------

nlohmann::json WsjcppJsonRpc20Request::getInputJson(const std::string &sParamName, nlohmann::json defaultValue) {
    // TODO check by input defs
    nlohmann::json jsonRet = defaultValue;
    if (m_jsonRequest[sParamName].is_structured()) {
        jsonRet = m_jsonRequest[sParamName];
    }
    return jsonRet;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20UserSession *WsjcppJsonRpc20Request::getUserSession() {
    return m_pWebSocketClient->getUserSession();
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isAdmin() {
    if (m_pWebSocketClient->getUserSession() != nullptr) {
        return m_pWebSocketClient->getUserSession()->isAdmin();
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isUser() {
    if (m_pWebSocketClient->getUserSession() != nullptr) {
        return m_pWebSocketClient->getUserSession()->isUser();
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::isUnauthorized() {
    return m_pWebSocketClient->getUserSession() == nullptr;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20Request::done(nlohmann::json& jsonResponseResult) {
    if (m_bResponseSend) {
        WsjcppLog::err(TAG, "Already sended response");
        return;
    }
    m_bResponseSend = true;
    // {"jsonrpc": "2.0", "method": "some" "result": {"hello": 5}, "id": "9"}
    nlohmann::json jsonResponse;
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = m_sMethodName;
    jsonResponse["id"] = m_sId;
    jsonResponse["result"] = jsonResponseResult;
    m_pWebSocketClient->sendTextMessage(jsonResponse.dump());
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20Request::fail(WsjcppJsonRpc20Error error) {
    if (m_bResponseSend) {
        WsjcppLog::err(TAG, "Already sended response");
        return;
    }
    m_bResponseSend = true;
    // {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request."}, "id": null}
    nlohmann::json jsonResponse;
    jsonResponse["jsonrpc"] = "2.0";
    jsonResponse["method"] = m_sMethodName;
    jsonResponse["id"] = m_sId;
    jsonResponse["error"] = error.toJson();
    m_pWebSocketClient->sendTextMessage(jsonResponse.dump());

    // m_pServer->sendMessage((QWebSocket*)m_pClient, jsonResponse);
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getId() {
    return m_sId;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20Request::getMethodName() {
    return m_sMethodName;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20Request::checkInputParams(const std::vector<WsjcppJsonRpc20ParamDef>& vParamDef) {
    // if (vParamDef.size() == 0) {
    //        return true;
    // }

    for (int i = 0; i < vParamDef.size(); i++) {
        WsjcppJsonRpc20ParamDef paramDef = vParamDef[i];


        bool bParamExists = m_jsonParams.find(paramDef.getName()) != m_jsonParams.end();
        if (paramDef.isRequired() && !bParamExists) {
                this->fail(WsjcppJsonRpc20Error(400, "PARAMETER_REQUIRED", {
                    {"method", this->getMethodName()},
                    {"param_name", paramDef.getName()}
                }));
                return false;
        }

        // TODO validators
        if (bParamExists) {
            if (paramDef.isString()) {
                if(!m_jsonParams[paramDef.getName()].is_string()) {
                    this->fail(WsjcppJsonRpc20Error(400, "DATATYPE_OF_PARAMETER_EXPECTED_STRING", {
                        {"method", m_sMethodName},
                        {"param_name", paramDef.getName()}
                    }));
                    return false;
                }

                std::string sVal = m_jsonParams[paramDef.getName()];
                std::string sError;
                const std::vector<WsjcppValidatorStringBase *> vValidators = paramDef.listOfStringValidators();
                for (int i = 0; i < vValidators.size(); i++) {
                    if (!vValidators[i]->isValid(sVal, sError)) {
                        this->fail(WsjcppJsonRpc20Error(400, "INVALID_PARAM_STRING_VALUE", {
                            {"method", m_sMethodName},
                            {"param_name", paramDef.getName()},
                            {"description", sError}
                        }));
                        return false;
                    }
                }

            } else if (paramDef.isBool() && !m_jsonParams[paramDef.getName()].is_boolean()) {
                this->fail(WsjcppJsonRpc20Error(400, "DATATYPE_OF_PARAMETER_EXPECTED_BOOLEAN", {
                    {"method", m_sMethodName},
                    {"param_name", paramDef.getName()}
                }));
                return false;
            } else if (paramDef.isInteger()) {

                if (!m_jsonParams[paramDef.getName()].is_number_integer()) {
                    this->fail(WsjcppJsonRpc20Error(400, "DATATYPE_OF_PARAMETER_EXPECTED_INTEGER", {
                        {"method", m_sMethodName},
                        {"param_name", paramDef.getName()}
                    }));
                    return false;
                }

                int nVal = m_jsonParams[paramDef.getName()];
                std::string sError;
                const std::vector<WsjcppValidatorIntegerBase *> vValidators = paramDef.listOfIntegerValidators();
                for (int i = 0; i < vValidators.size(); i++) {
                    if (!vValidators[i]->isValid(nVal, sError)) {
                        this->fail(WsjcppJsonRpc20Error(400, "INVALID_PARAM_INTEGER_VALUE", {
                            {"method", m_sMethodName},
                            {"param_name", paramDef.getName()},
                            {"description", sError}
                        }));
                        return false;
                    }
                }
                
            } else if (paramDef.isJson()) {
                if (!m_jsonParams[paramDef.getName()].is_structured()) {
                    this->fail(WsjcppJsonRpc20Error(400, "DATATYPE_OF_PARAMETER_EXPECTED_JSON_OBJECT", {
                        {"method", m_sMethodName},
                        {"param_name", paramDef.getName()}
                    }));
                    return false;
                }
                nlohmann::json jsonVal = m_jsonParams[paramDef.getName()];
                std::string sError;

                const std::vector<WsjcppValidatorJsonBase *> vValidators = paramDef.listOfJsonValidators();
                for (int i = 0; i < vValidators.size(); i++) {
                    if (!vValidators[i]->isValid(jsonVal, sError)) {
                        this->fail(WsjcppJsonRpc20Error(400, "INVALID_PARAM_JSON_VALUE", {
                            {"method", m_sMethodName},
                            {"param_name", paramDef.getName()},
                            {"description", sError}
                        }));
                        return false;
                    }
                }
            }
        }
    }

    // check extra params
    for (auto it = m_jsonParams.begin(); it != m_jsonParams.end(); ++it) {
        std::string sKey = it.key();
        bool bFound = false;
        for (int i = 0; i < vParamDef.size(); i++) {
            if (vParamDef[i].getName() == sKey) {
                bFound = true;
            }
        }
        if (!bFound) {
            this->fail(WsjcppJsonRpc20Error(400, "EXTRA_PARAMETER_FOUND", {
                {"method", m_sMethodName},
                {"param_name", sKey}
            }));
            return false;
        }
    }

    /*try {
        
        for (WsjcppJsonRpc20ParamDef inDef : pCmdHandler->inputs()) {
            auto itJsonParamName = jsonMessage.find(inDef.getName());
         

            if (itJsonParamName != endJson) {
                if (itJsonParamName->is_null()) {
                    error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' could not be null");
                    return false;
                }

                if (inDef.isInteger()) {
                    if (!itJsonParamName->is_number()) {
                        error = WsjcppJsonRpc20Error(400, "Parameter '" + inDef.getName() + "' must be integer");
                        return false;
                    }
                }

                if (inDef.isString()) {
                    std::string sVal = itJsonParamName->get_ref<std::string const&>();
                    std::string sError;
                    const std::vector<WsjcppValidatorStringBase *> vValidators = inDef.listOfStringValidators();
                    for (int i = 0; i < vValidators.size(); i++) {
                        if (!vValidators[i]->isValid(sVal, sError)) {
                            error = WsjcppJsonRpc20Error(400, "Wrong param '" + inDef.getName() + "': " + sError);
                            return false;
                        }
                    }
                }

                if (inDef.isInteger()) {
                    int nVal = *itJsonParamName;
                    std::string sError;
                    const std::vector<WsjcppValidatorIntegerBase *> vValidators = inDef.listOfIntegerValidators();
                    for (int i = 0; i < vValidators.size(); i++) {
                        if (!vValidators[i]->isValid(nVal, sError)) {
                            error = WsjcppJsonRpc20Error(400, "Wrong param '" + inDef.getName() + "': " + sError);
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    } catch(std::exception const &e) {
        error = WsjcppJsonRpc20Error(500, "InternalServerError");
        WsjcppLog::err(TAG, std::string("validateInputParameters, ") + e.what());
        return false;
    }*/
    return true;
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20HandlerBase

WsjcppJsonRpc20HandlerBase::WsjcppJsonRpc20HandlerBase(const std::string &sMethodName, const std::string &sDescription) {
    m_sMethodName = sMethodName;
    m_sDescription = sDescription;
    TAG = "WsjcppJsonRpc20HandlerBase(" + sMethodName + ")";

    m_bAccessUnauthorized = false;
    m_bAccessUser = false;
    m_bAccessTester = false;
    m_bAccessAdmin = false;

    // can register in a global list
    WsjcppJsonRpc20::addHandler(sMethodName, this);
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20HandlerBase::getActivatedFromVersion() const {
    return m_sActivatedFromVersion;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20HandlerBase::getDeprecatedFromVersion() const {
    return m_sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20HandlerBase::haveUnauthorizedAccess() const {
    return m_bAccessUnauthorized;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20HandlerBase::haveUserAccess() const {
    return m_bAccessUser;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20HandlerBase::haveTesterAccess() const {
    return m_bAccessTester;
}

// ---------------------------------------------------------------------

bool WsjcppJsonRpc20HandlerBase::haveAdminAccess() const {
    return m_bAccessAdmin;
}

// ---------------------------------------------------------------------
// TODO write unit-test for this

bool WsjcppJsonRpc20HandlerBase::checkAccess(WsjcppJsonRpc20Request *pRequest) const {
    WsjcppJsonRpc20UserSession *pUserSession = pRequest->getUserSession();
    if (!haveUnauthorizedAccess()) {
        if (pUserSession == nullptr) {
            pRequest->fail(WsjcppJsonRpc20Error(401, "NOT_AUTHORIZED_REQUEST"));
            return false;
        }

        // access user
        if (pUserSession->isUser() && !haveUserAccess()) {
            pRequest->fail(WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_USER"));
            return false;
        }

        // access tester
        if (pUserSession->isTester() && !haveTesterAccess()) {
            pRequest->fail(WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_TESTER"));
            return false;
        }

        // access admin
        if (pUserSession->isAdmin() && !haveAdminAccess()) {
            pRequest->fail(WsjcppJsonRpc20Error(403, "ACCESS_DENY_FOR_ADMIN"));
            return false;
        }
    }

    return true;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppJsonRpc20ParamDef> &WsjcppJsonRpc20HandlerBase::getParamsDef() {
    return m_vParamsDef;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20HandlerBase::getMethodName() const {
    return m_sMethodName;
}

// ---------------------------------------------------------------------

std::string WsjcppJsonRpc20HandlerBase::getDescription() const {
    return m_sDescription;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setAccessUnauthorized(bool bAccess) {
    m_bAccessUnauthorized = bAccess;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setAccessUser(bool bAccess) {
    m_bAccessUser = bAccess;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setAccessTester(bool bAccess) {
    m_bAccessTester = bAccess;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setAccessAdmin(bool bAccess) {
    m_bAccessAdmin = bAccess;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setActivatedFromVersion(const std::string &sActivatedFromVersion) {
    m_sActivatedFromVersion = sActivatedFromVersion;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion) {
    m_sDeprecatedFromVersion = sDeprecatedFromVersion;
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::requireStringParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pStringDef(sName, sDescription);
    pStringDef.string_().required();
    m_vParamsDef.push_back(pStringDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::optionalStringParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pStringDef(sName, sDescription);
    pStringDef.string_().optional();
    m_vParamsDef.push_back(pStringDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::requireIntegerParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().required();
    m_vParamsDef.push_back(pIntegerDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::optionalIntegerParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pIntegerDef(sName, sDescription);
    pIntegerDef.integer_().optional();
    m_vParamsDef.push_back(pIntegerDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::requireBooleanParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().required();
    m_vParamsDef.push_back(pBooleanDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::optionalBooleanParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pBooleanDef(sName, sDescription);
    pBooleanDef.bool_().optional();
    m_vParamsDef.push_back(pBooleanDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::requireJsonParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pJsonDef(sName, sDescription);
    pJsonDef.json_().required();
    m_vParamsDef.push_back(pJsonDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20ParamDef &WsjcppJsonRpc20HandlerBase::optionalJsonParam(const std::string &sName, const std::string &sDescription) {
    this->validateParamName(sName);
    WsjcppJsonRpc20ParamDef pJsonDef(sName, sDescription);
    pJsonDef.json_().optional();
    m_vParamsDef.push_back(pJsonDef);
    return m_vParamsDef[m_vParamsDef.size()-1];
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerBase::validateParamName(const std::string &sName) {
    std::regex rxName("[a-z]+[0-9a-z_]*");
    if (!std::regex_match(sName, rxName)) {
        WsjcppLog::throw_err(TAG, "Parameter '" + sName + "' in method '" + m_sMethodName + "' has invalid format. Expected '[a-z]+[0-9a-z_]*'");
    }
    for (int i = 0; i < m_vParamsDef.size(); i++) {
        if (m_vParamsDef[i].getName() == sName) {
            WsjcppLog::throw_err(TAG, "Parameter '" + sName + "' in method '" + m_sMethodName + "' already exists");
        }
    }
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20

std::map<std::string, WsjcppJsonRpc20HandlerBase*> *g_pWsjcppJsonRpc20HandlerList = nullptr;

void WsjcppJsonRpc20::initGlobalVariables() {
    if (g_pWsjcppJsonRpc20HandlerList == nullptr) {
        // WsjcppLog::info(std::string(), "Create handlers map");
        g_pWsjcppJsonRpc20HandlerList = new std::map<std::string, WsjcppJsonRpc20HandlerBase*>();
    }
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20::addHandler(const std::string &sCmd, WsjcppJsonRpc20HandlerBase* pCmdHandler) {
    WsjcppJsonRpc20::initGlobalVariables();
    if (g_pWsjcppJsonRpc20HandlerList->count(sCmd)) {
        WsjcppLog::err(sCmd, "Already registered");
    } else {
        g_pWsjcppJsonRpc20HandlerList->insert(std::pair<std::string, WsjcppJsonRpc20HandlerBase*>(sCmd,pCmdHandler));
        // WsjcppLog::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

WsjcppJsonRpc20HandlerBase * WsjcppJsonRpc20::findJsonRpc20Handler(const std::string &sMethodName) {
    WsjcppJsonRpc20::initGlobalVariables();
    WsjcppJsonRpc20HandlerBase *pHandler = nullptr;

    std::map<std::string, WsjcppJsonRpc20HandlerBase*>::iterator it;
    it = g_pWsjcppJsonRpc20HandlerList->find(sMethodName);

    if (it != g_pWsjcppJsonRpc20HandlerList->end()) {
        pHandler = it->second;
    }

    if (pHandler == nullptr) {
        WsjcppLog::err(sMethodName, "Not found");
    }

    return pHandler;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppJsonRpc20::getEventList() {
    std::vector<std::string> vEvents;
    // TODO
    return vEvents;
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20::registryEventFabric(const std::string &sEventName, WsjcppJsonRpc20EventBase* pEventFabric) {
    // TODO
}

// ---------------------------------------------------------------------
// WsjcppJsonRpc20HandlerServerApi

REGISTRY_WSJCPP_JSONRPC20_HANDLER(WsjcppJsonRpc20HandlerServerApi)

WsjcppJsonRpc20HandlerServerApi::WsjcppJsonRpc20HandlerServerApi()
    : WsjcppJsonRpc20HandlerBase("server_api", "This method Will be return list of all handlers") {

    setAccessUnauthorized(true);
    setAccessTester(true);
    setAccessUser(true);
    setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void WsjcppJsonRpc20HandlerServerApi::handle(WsjcppJsonRpc20Request *pRequest) {
    nlohmann::json jsonResponse;
    jsonResponse["version"] = WSJCPP_APP_VERSION;

    // EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    // int nWsPort = pGlobalSettings->get("port").getNumberValue();
    // bool bSslOn = pGlobalSettings->get("ssl_on").getBooleanValue();
    // int nWssPort = pGlobalSettings->get("ssl_port").getNumberValue();

    // jsonResponse["server_ws_port"] = pGlobalSettings->get("port").getNumberValue();
    // if (bSslOn) {
    //        jsonResponse["server_wss_port"] = nWssPort;
    //    }

    nlohmann::json jsonHandlers = nlohmann::json::array();
    std::map<std::string, WsjcppJsonRpc20HandlerBase *>::iterator it = g_pWsjcppJsonRpc20HandlerList->begin();
    int nDataLength = 0;
    while (it != g_pWsjcppJsonRpc20HandlerList->end()) {
        std::string sCmd = it->first;
        WsjcppJsonRpc20HandlerBase *pHandler = g_pWsjcppJsonRpc20HandlerList->at(sCmd);

        nlohmann::json jsonHandler;

        jsonHandler["method"] = pHandler->getMethodName();
        jsonHandler["description"] = pHandler->getDescription();

        nlohmann::json jsonAccess;
        jsonAccess["unauthorized"] = pHandler->haveUnauthorizedAccess();
        jsonAccess["user"] = pHandler->haveUserAccess();
        jsonAccess["tester"] = pHandler->haveTesterAccess();
        jsonAccess["admin"] = pHandler->haveAdminAccess();
        jsonHandler["access"] = jsonAccess;

        nlohmann::json jsonInputs = nlohmann::json::array();
        std::vector<WsjcppJsonRpc20ParamDef> ins = pHandler->getParamsDef();
        if (ins.size() > 0) {
            for (unsigned int i = 0; i < ins.size(); i++) {
                jsonInputs.push_back(ins[i].toJson());
            }
            jsonHandler["params"] = jsonInputs;
        }
        jsonHandlers.push_back(jsonHandler);
        it++;
        nDataLength++;
    }
    
    jsonResponse["data"] = jsonHandlers;
    jsonResponse["data_length"] = nDataLength;
    pRequest->done(jsonResponse);
}
