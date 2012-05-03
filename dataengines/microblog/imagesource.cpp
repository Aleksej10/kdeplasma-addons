/*
 *   Copyright 2008 Aaron Seigo <aseigo@kde.org>
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

#include "imagesource.h"


#include <KIO/Job>

ImageSource::ImageSource(QObject* parent)
    : Plasma::DataContainer(parent),
      m_runningJobs(0)
{
    setObjectName(QLatin1String("UserImages"));
}

ImageSource::~ImageSource()
{
}

void ImageSource::loadStarted()
{
    m_cachedData = data();
    removeAllData();
}

void ImageSource::loadFinished()
{
    m_cachedData.clear();
}

void ImageSource::loadImage(const QString &who, const KUrl &url)
{
    //FIXME: since kio_http bombs the system with too many request put a temporary arbitrary limit here
    // revert as soon as BUG 192625 is fixed
    if (m_runningJobs < 5) {
        if (m_cachedData.contains(who)) {
            kDebug() << "UserImage:" << who;
            setData(who, m_cachedData.value(who));
        }

        m_runningJobs++;
        KIO::Job *job = KIO::get(url, KIO::NoReload, KIO::HideProgressInfo);
        job->setAutoDelete(true);
        m_jobs[job] = who;
        connect(job, SIGNAL(data(KIO::Job*,QByteArray)),
                this, SLOT(recv(KIO::Job*,QByteArray)));
        connect(job, SIGNAL(result(KJob*)), this, SLOT(result(KJob*)));
    } else {
        m_queuedJobs.append(QPair<QString, KUrl>(who, url));
    }
}

void ImageSource::recv(KIO::Job* job, const QByteArray& data)
{
    m_jobData[job] += data;
    //kDebug() << m_data;
}

void ImageSource::result(KJob *job)
{
    if (!m_jobs.contains(job)) {
        return;
    }

    m_runningJobs--;

    if (m_queuedJobs.count() > 0) {
        QPair<QString, KUrl> jobDesc = m_queuedJobs.takeLast();
        loadImage(jobDesc.first, jobDesc.second);
    }

    if (job->error()) {
        // TODO: error handling
    } else {
        //kDebug() << "done!" << m_jobData;
        QImage img;
        img.loadFromData(m_jobData.value(job));
        kDebug() << "UserImage:" << m_jobs.value(job);
        setData(m_jobs.value(job), img);
        emit dataChanged();
    }

    m_jobs.remove(job);
    m_jobData.remove(job);
    checkForUpdate();
}

Plasma::DataEngine::Data ImageSource::data()
{
    return m_cachedData;
}


#include <imagesource.moc>

