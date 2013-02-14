#ifndef JIRA_XS_H
#define JIRA_XS_H

BEGIN_EASYSOAP_NAMESPACE

template<>
class SOAPTypeTraits<JIRA::RemoteWorklog>
{
public:
    static void GetType(SOAPQName& qname)
    {
        qname.Set("RemoteWorklog", "tns1:RemoteWorklog");
    }

    static SOAPParameter& Serialize(SOAPParameter& p, const JIRA::RemoteWorklog& v)
    {
        if(v.author.length() > 0)
        {
            p.AddParameter("author") << v.author;
        }
        else
        {
            p.AddParameter("author").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.comment.length() > 0)
        {
            p.AddParameter("comment") << v.comment;
        }
        else
        {
           p.AddParameter("comment").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.created.length() > 0)
        {
            (p.AddParameter("created") << v.created).SetType("dateTime", XMLSchema2001::xsd);
        }
        else
        {
            p.AddParameter("created").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.groupLevel.length() > 0)
        {
            p.AddParameter("groupLevel") << v.groupLevel;
        }
        else
        {
            p.AddParameter("groupLevel").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.id.length() > 0)
        {
            p.AddParameter("id") << v.id;
        }
        else
        {
            p.AddParameter("id").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.roleLevelId.length() > 0)
        {
            p.AddParameter("roleLevelId") << v.roleLevelId;
        }
        else
        {
            p.AddParameter("roleLevelId").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.startDate.length() > 0)
        {
            (p.AddParameter("startDate") << v.startDate).SetType("dateTime", XMLSchema2001::xsd);
        }
        else
        {
            p.AddParameter("startDate").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.timeSpent.length() > 0)
        {
            p.AddParameter("timeSpent") << v.timeSpent;
        }
        else
        {
            p.AddParameter("timeSpent").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.timeSpentInSeconds > 0)
        {
            p.AddParameter("timeSpentInSeconds") << v.timeSpentInSeconds;
        }
        else
        {
            p.AddParameter("timeSpentInSeconds").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.updateAuthor.length() > 0)
        {
            p.AddParameter("updateAuthor") << v.updateAuthor;
        }
        else
        {
            p.AddParameter("updateAuthor").AddAttribute(XMLSchema2001::nil) = "true";
        }

        if(v.updated.length() > 0)
        {
            (p.AddParameter("updated") << v.updated).SetType("dateTime", XMLSchema2001::xsd);
        }
        else
        {
            p.AddParameter("updated").AddAttribute(XMLSchema2001::nil) = "true";
        }

        return p;
    }

    static const SOAPParameter& Deserialize(const SOAPParameter& p, JIRA::RemoteWorklog& v)
    {
        p.GetParameter("author") >> v.author;
        p.GetParameter("comment") >> v.comment;
        p.GetParameter("created") >> v.created;
        p.GetParameter("groupLevel") >> v.groupLevel;
        p.GetParameter("id") >> v.id;
        p.GetParameter("roleLevelId") >> v.roleLevelId;
        p.GetParameter("startDate") >> v.startDate;
        p.GetParameter("timeSpent") >> v.timeSpent;
        p.GetParameter("timeSpentInSeconds") >> v.timeSpentInSeconds;
        p.GetParameter("updateAuthor") >> v.updateAuthor;
        p.GetParameter("updated") >> v.updated;

        return p;
    }
};

END_EASYSOAP_NAMESPACE

#endif // JIRA_XS_H
