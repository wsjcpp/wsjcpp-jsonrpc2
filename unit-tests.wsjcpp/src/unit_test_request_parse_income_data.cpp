#include "unit_test_request_parse_income_data.h"
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_jsonrpc20.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestRequestParseIncomeData)

UnitTestRequestParseIncomeData::UnitTestRequestParseIncomeData()
    : WsjcppUnitTestBase("UnitTestRequestParseIncomeData") {
}

// ---------------------------------------------------------------------

void UnitTestRequestParseIncomeData::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestRequestParseIncomeData::run() {
    bool bTestSuccess = true;
    WsjcppJsonRpc20WebSocketServer *pWebSocketServer = new WsjcppJsonRpc20WebSocketServer();
    FakeWebSocketClient *pFakeClient = new FakeWebSocketClient();
    WsjcppJsonRpc20WebSocketClient *pClient = pFakeClient;
    
    // parseIncomeData empty - wrong json
    {
        pFakeClient->clearLastTextMessage();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData empty", pRequest->parseIncomeData(""), false);
        std::string sResponse0 = pFakeClient->getLastTextMessage();
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse0);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData empty. error.message", sErrorMessage, "WRONG_JSON");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData empty. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData empty. id", sId, "unknown_id");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData empty. method", sMethod, "unknown_method");
    }
    
    // missing 'id' in request
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "game_create";
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData missing id", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData missing id. error.message", sErrorMessage, "NOT_FOUND_ID_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData missing id. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData missing id. id", sId, "unknown_id");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData missing id. method", sMethod, "game_create");
    }

    // missing 'method' in request
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["id"] = "id1";
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData missing method", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        // std::cout << sResponse << std::endl;
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData missing method. error. message", sErrorMessage, "NOT_FOUND_METHOD_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData missing method. error. code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData missing method. id", sId, "id1");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData missing method. method", sMethod, "unknown_method");
    }

    // missing 'jsonrpc' in request
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["id"] = "id1";
        jsonRequest["method"] = "game_create";
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData missing jsonrpc", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        // std::cout << sResponse << std::endl;
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData missing jsonrpc. error.message", sErrorMessage, "NOT_FOUND_FIELD_JSONRPC_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData missing jsonrpc. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData missing jsonrpc. id", sId, "id1");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData missing jsonrpc. method", sMethod, "game_create");
    }

    // field 'jsonrpc' is non string in request
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["id"] = "id1";
        jsonRequest["jsonrpc"] = 2;
        jsonRequest["method"] = "game_create";
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData jsonrpc is non string", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        // std::cout << sResponse << std::endl;
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is non string. error.message", sErrorMessage, "FIELD_JSONRPC_EXPECTED_AS_STRING_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData jsonrpc is non string. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is non string. id", sId, "id1");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is non string. method", sMethod, "game_create");
    }

    // field 'jsonrpc' is no "2.0" in request
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["id"] = "id1";
        jsonRequest["jsonrpc"] = "2.1";
        jsonRequest["method"] = "game_create";
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData jsonrpc is no 2.0", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        // std::cout << sResponse << std::endl;
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is no 2.0. error.message", sErrorMessage, "FIELD_JSONRPC_EXPECTED_2_DOT_0_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData jsonrpc is no 2.0. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is no 2.0. id", sId, "id1");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData jsonrpc is no 2.0. method", sMethod, "game_create");
    }

    // field 'params' wrong
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["id"] = "id1";
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "game_create";
        jsonRequest["params"] = nlohmann::json::array();
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData field params wrong", pRequest->parseIncomeData(sRequest), false);
        std::string sResponse = pFakeClient->getLastTextMessage();
        // std::cout << sResponse << std::endl;
        nlohmann::json jsonResponse = nlohmann::json::parse(sResponse);
        std::string sErrorMessage = jsonResponse["error"]["message"];
        compareS(bTestSuccess, "parseIncomeData field params wrong. error.message", sErrorMessage, "FIELD_PARAMS_EXPECTED_AS_OBJECT_IN_REQUEST");
        int nErrorCode = jsonResponse["error"]["code"];
        compareN(bTestSuccess, "parseIncomeData field params wrong. error.code", nErrorCode, 400);
        std::string sId = jsonResponse["id"];
        compareS(bTestSuccess, "parseIncomeData field params wrong. id", sId, "id1");
        std::string sMethod = jsonResponse["method"];
        compareS(bTestSuccess, "parseIncomeData field params wrong. method", sMethod, "game_create");
    }

    // success parsing
    {
        pFakeClient->clearLastTextMessage();
        nlohmann::json jsonRequest;
        jsonRequest["id"] = "id1";
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "game_create";
        jsonRequest["params"] = nlohmann::json::object();
        std::string sRequest = jsonRequest.dump();
        WsjcppJsonRpc20Request *pRequest = new WsjcppJsonRpc20Request(pClient, pWebSocketServer);
        compareB(bTestSuccess, "parseIncomeData success parsing", pRequest->parseIncomeData(sRequest), true);
        std::string sResponse = pFakeClient->getLastTextMessage();
        compareS(bTestSuccess, "parseIncomeData success parsing. error must be empty", sResponse, "");
    }
    return bTestSuccess;
}
