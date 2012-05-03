/*
 *   Copyright 2008 Aaron Seigo <aseigo@kde.org>
 *   Copyright 2012 Sebastian Kügler <sebas@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "usersource.h"


#include <KIO/Job>

#include <QXmlStreamReader>

UserSource::UserSource(const QString &who, const QString &serviceBaseUrl, QObject* parent)
    : Plasma::DataContainer(parent),
      m_user(who),
      m_serviceBaseUrl(serviceBaseUrl)
{
    setObjectName(QLatin1String("User"));
    loadUserInfo(who, m_serviceBaseUrl);
}

UserSource::~UserSource()
{
}

void UserSource::loadUserInfo(const QString &who, const KUrl &serviceBaseUrl)
{
    if (who.isEmpty() || serviceBaseUrl.isEmpty()) {
        return;
    }
//     if (m_cachedData.contains(who)) {
//         kDebug() << "UserInfo:" << who;
//         setData(who, m_cachedData.value(who));
//     }
    const QString u = QString("%1/users/show/%2.xml").arg(serviceBaseUrl.pathOrUrl(), who);
    kDebug() << "Requesting user info from: " << u;
    //return;
    //m_runningJobs++;
    KIO::Job *job = KIO::get(u, KIO::NoReload, KIO::HideProgressInfo);
    job->setAutoDelete(true);
    m_jobs[job] = who;
    connect(job, SIGNAL(data(KIO::Job*,QByteArray)),
            this, SLOT(recv(KIO::Job*,QByteArray)));
    connect(job, SIGNAL(result(KJob*)), this, SLOT(result(KJob*)));
}

void UserSource::recv(KIO::Job* job, const QByteArray& data)
{
    m_xml += data;
}

void UserSource::result(KJob *job)
{
    if (!m_jobs.contains(job)) {
        return;
    }

    if (job->error()) {
        // TODO: error handling
    } else {
        QXmlStreamReader reader(m_xml);
        parse(reader);
        checkForUpdate();
        m_xml.clear();
    }

    m_jobs.remove(job);
    m_jobData.remove(job);
    checkForUpdate();
}


void UserSource::parse(QXmlStreamReader &xml)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            QString tag = xml.name().toString().toLower();

            if (tag == "status") {
                //readStatus(xml);
            } else if (tag == "user") {
                kDebug() << "Found user.";
                readUser(xml);
            } else if (tag == "direct_message") {
                //readDirectMessage(xml);
            }
        }
    }

    if (xml.hasError()) {
        kWarning() << "Fatal error on line" << xml.lineNumber()
                   << ", column" << xml.columnNumber() << ":"
                   << xml.errorString();
    }
}

void UserSource::readUser(QXmlStreamReader &xml)
{
    QHash<QString, QString> tagKeys;
    tagKeys.insert("id", "userid");
    tagKeys.insert("screen_name", "username");
    tagKeys.insert("name", "realname");
    tagKeys.insert("location", "location");
    tagKeys.insert("description", "description");
    tagKeys.insert("protected", "protected");
    tagKeys.insert("followers_count", "followers");
    tagKeys.insert("friends_count", "friends");
    tagKeys.insert("statuses_count", "tweets");
    tagKeys.insert("time_zone", "timezone");
    tagKeys.insert("utc_offset", "utcoffset");
    tagKeys.insert("profile_image_url", "profileimageurl");
    tagKeys.insert("statusnet:profile_url", "profileurl");
    tagKeys.insert("url", "url");
    tagKeys.insert("following", "following");
    tagKeys.insert("notifications", "notifications");
    tagKeys.insert("statusnet:blocking", "blocking");
    tagKeys.insert("created_at", "created");

    kDebug() << "- BEGIN USER -" << endl;
    const QString tagName("user");

    while (!xml.atEnd()) {
        xml.readNext();
        //kDebug() << "next el";
        QString tag = xml.name().toString().toLower();

        if (xml.isEndElement() && tag == tagName) {
            break;
        }

        if (xml.isStartElement()) {
            QString cdata;

            if (tag == "status") {
                //readStatus(xml);
            } else {
                cdata = xml.readElementText(QXmlStreamReader::IncludeChildElements).trimmed();
                if (tagKeys.keys().contains(tag)) {
                    setData(tagKeys[tag], cdata);
                }
            }
        }
    }

    // Make sure our avatar is loaded
    kDebug() << "requesting profile pic" << data()["username"] << data()["profileimageurl"];
    //const QString who = 
    emit loadImage(data()["username"].toString(), data()["profileimageurl"].toUrl());

    //kDebug() << "- END USER -" << endl;
}

#include <usersource.moc>

