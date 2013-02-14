#include "jira.h"
#include <easysoap/SOAPSTL.h>

#include "jira_xs.h"

const char *JIRA::namespaceURI = "http://jira.atlassian.com/rpc/soap/jirasoapservice-v2";
const char *JIRA::defaultEndpoint = "http://localhost:8080/rpc/soap/jirasoapservice-v2";

JIRA::JIRA(const char *endpoint)
{
    if (endpoint)
        _proxy.SetEndpoint(endpoint);
    else
        _proxy.SetEndpoint(defaultEndpoint);
}

void JIRA::login(const std::string &userName, const std::string &password, std::string &loginReturn)
{
    SOAPMethod method("login", namespaceURI);

    method.AddParameter("in0") << userName;
    method.AddParameter("in1") << password;

    const SOAPResponse& response = _proxy.Execute(method);
    response.GetReturnValue() >> loginReturn;
}

void JIRA::logout(const std::string &token, bool &logoutReturn)
{
    SOAPMethod method("logout", namespaceURI);

    method.AddParameter("in0") << token;

    const SOAPResponse& response = _proxy.Execute(method);
    response.GetReturnValue() >> logoutReturn;
}

void JIRA::addWorklogAndAutoAdjustRemainingEstimate(const std::string &token, const std::string &issue,
                                                    const RemoteWorklog &worklog,
                                                    RemoteWorklog &addWorklogAndAutoAdjustRemainingEstimateReturn)
{
    SOAPMethod method("addWorklogAndAutoAdjustRemainingEstimate", namespaceURI);

    method.AddParameter("in0") << token;
    method.AddParameter("in1") << issue;
    method.AddParameter("in2") << worklog;

    const SOAPResponse& response = _proxy.Execute(method);
    response.GetReturnValue() >> addWorklogAndAutoAdjustRemainingEstimateReturn;
}
