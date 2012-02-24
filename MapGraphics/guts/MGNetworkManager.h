#ifndef MGNETWORKMANAGER_H
#define MGNETWORKMANAGER_H

#include "MapGraphics_global.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>

/**
 * @brief This is a singleton class that wraps a QNetworkAccessManager.
 * It exists so that everybody can easily use the same QNetworkAccessManager and so that
 * we can configure request caching in a central place.
 *
 */
class MAPGRAPHICSSHARED_EXPORT MGNetworkManager
{
public:
    /**
     * @brief Returns the one-and-only instance of MGNetworkManager.
     *
     */
    static MGNetworkManager * getInstance();

    virtual ~MGNetworkManager();

    /**
     * @brief Given a QNetworkRequest, starts performing the request and returns a
     * QNetworkReply which can be used to learn about the status and results of the request
     * operation.
     *
     * @param request
     */
    QNetworkReply * get(QNetworkRequest &request);

    /**
     * @brief Configure whether or not you want network requests to be cached.
     * Requests are cached by default unless you explicitly disable them.
     * You probably don't want this.
     *
     * @param enable
     */
    void setCachingEnabled(bool enable);

    /**
     * @brief Returns true if caching is enabled, false otherwise
     *
     */
    bool cachingEnabled() const;

    void setUserAgent(const QByteArray& agent);
    QByteArray userAgent() const;

protected:
    MGNetworkManager();

private:
    static MGNetworkManager * instance;
    QNetworkAccessManager * manager;

    QByteArray _userAgent;

};

#endif // MGNETWORKMANAGER_H
