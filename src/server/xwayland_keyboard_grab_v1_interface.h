/****************************************************************************
Copyright 2017  David Edmundson <kde@davidedmundson.co.uk>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#ifndef KWAYLAND_SERVER_XWAYLAND_KEYBOARD_INTERFACE_H
#define KWAYLAND_SERVER_XWAYLAND_KEYBOARD_INTERFACE_H

#include <DWayland/Server/kwaylandserver_export.h>

#include <QObject>

struct wl_resource;

namespace KWaylandServer
{

class Display;
class SurfaceInterface;
class SeatInterface;
class ZWPXwaylandKeyboardGrabV1Interface;
class ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate;
class ZWPXwaylandKeyboardGrabV1InterfacePrivate;

/**
 * Provides the DBus service name and object path to a ZWPXwaylandKeyboardGrabV1 DBus interface.
 *
 * This global can be used for clients to bind AppmenuInterface instances
 * and notifies when a new one is created
 * @since 5.42
 */
class KWAYLANDSERVER_EXPORT ZWPXwaylandKeyboardGrabManagerV1Interface : public QObject
{
    Q_OBJECT

public:
    explicit ZWPXwaylandKeyboardGrabManagerV1Interface(Display *display, QObject *parent = nullptr);
    ~ZWPXwaylandKeyboardGrabManagerV1Interface() override;

    ZWPXwaylandKeyboardGrabV1Interface* getGrabClient();

Q_SIGNALS:
    /**
     * Emitted whenever a new AppmenuInterface is created.
     **/
    void zwpXwaylandKeyboardGrabV1Created(KWaylandServer::ZWPXwaylandKeyboardGrabV1Interface *grab);
    /**
     * Emitted whenever a new AppmenuInterface is created.
     **/
    void zwpXwaylandKeyboardGrabV1Destroyed();

private:
    QScopedPointer<ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate> d;
};

/**
 * Provides the DBus service name and object path to a ZWPXwaylandKeyboardGrabV1 DBus interface.
 * This interface is attached to a wl_surface and provides access to where
 * the ZWPXwaylandKeyboardGrabV1 DBus interface is registered.
 * @since 5.42
 */
class KWAYLANDSERVER_EXPORT ZWPXwaylandKeyboardGrabV1Interface : public QObject
{
    Q_OBJECT
public:
    enum GrabType {
        GrabServer,
        GrabKeyboard,
        GrabPointer
    };
    ZWPXwaylandKeyboardGrabV1Interface(ZWPXwaylandKeyboardGrabManagerV1Interface *parent, SurfaceInterface *s, SeatInterface *seat, wl_resource *resource);
    ~ZWPXwaylandKeyboardGrabV1Interface() override;

    /**
     * @returns The SurfaceInterface this AppmenuInterface references.
     **/
    SurfaceInterface *surface() const;

    SeatInterface *seat() const;

private:
    friend class ZWPXwaylandKeyboardGrabManagerV1Interface;

    QScopedPointer<ZWPXwaylandKeyboardGrabV1InterfacePrivate> d;
};


}

#endif
