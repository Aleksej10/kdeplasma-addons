/*
 *   SPDX-FileCopyrightText: 2008 Petri Damstén <damu@iki.fi>
 *
 *   SPDX-License-Identifier: LGPL-2.0-only
 */

#ifndef COMICPROVIDERKROSS_H
#define COMICPROVIDERKROSS_H

#include "comicprovider.h"
#include "comicproviderwrapper.h"

#include <QImage>
#include <QUrl>
#include <KPackage/PackageStructure>

class ComicProviderKross : public ComicProvider
{
        friend class ComicProviderWrapper;
        Q_OBJECT

    public:
        ComicProviderKross(QObject *parent, const QVariantList &args);
        ~ComicProviderKross() override;

        static KPackage::PackageStructure *packageStructure();

        bool isLeftToRight() const override;
        bool isTopToBottom() const override;
        IdentifierType identifierType() const override;
        QUrl websiteUrl() const override;
        QUrl shopUrl() const override;
        QImage image() const override;
        QString identifier() const override;
        QString nextIdentifier() const override;
        QString previousIdentifier() const override;
        QString firstStripIdentifier() const override;
        QString stripTitle() const override;
        QString additionalText() const override;

    protected:
        void pageRetrieved(int id, const QByteArray &data) override;
        void pageError(int id, const QString &message) override;
        void redirected(int id, const QUrl &newUrl) override;
        QString identifierToString(const QVariant &identifier) const;

    private:
        mutable ComicProviderWrapper m_wrapper;
        static KPackage::PackageStructure *m_packageStructure;
};

#endif
