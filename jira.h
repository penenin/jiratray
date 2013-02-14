#ifndef JIRA_H
#define JIRA_H

#include <string>
#include <easysoap/SOAP.h>

USING_EASYSOAP_NAMESPACE

class JIRA
{
private:
        static const char *namespaceURI;
        static const char *defaultEndpoint;

        JIRA(const JIRA&);
        JIRA& operator=(const JIRA&);

        SOAPProxy _proxy;

public:
        typedef struct
        {
          std::string author;
          std::string comment;
          std::string created;
          std::string groupLevel;
          std::string id;
          std::string roleLevelId;
          std::string startDate;
          std::string timeSpent;
          long timeSpentInSeconds;
          std::string updateAuthor;
          std::string updated;
        } RemoteWorklog;

        JIRA(const char *endpoint = 0);

        void login(const std::string& userName, const std::string& password, std::string& loginReturn);
        void logout(const std::string& token, bool& logoutReturn);
        void addWorklogAndAutoAdjustRemainingEstimate(const std::string& token, const std::string& issue,
                                                      const RemoteWorklog& worklog,
                                                      RemoteWorklog& addWorklogAndAutoAdjustRemainingEstimateReturn);
};

#endif // JIRA_H
