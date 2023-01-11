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
#include "xwayland_keyboard_grab_v1_interface.h"
#include "display.h"
#include "surface_interface.h"
#include "seat_interface.h"
#include "logging.h"

#include "qwayland-server-xwayland-keyboard-grab-unstable-v1.h"

namespace KWaylandServer
{

static const quint32 s_version = 1;

class ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate : public QtWaylandServer::zwp_xwayland_keyboard_grab_manager_v1
{
public:

    ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate(ZWPXwaylandKeyboardGrabManagerV1Interface *q, Display *display);

    ZWPXwaylandKeyboardGrabV1Interface* grab;
    ZWPXwaylandKeyboardGrabManagerV1Interface *q;

protected:
    void zwp_xwayland_keyboard_grab_manager_v1_destroy(Resource *resource) override;
    void zwp_xwayland_keyboard_grab_manager_v1_grab_keyboard(Resource *resource, uint32_t id, struct ::wl_resource *surface, struct ::wl_resource *seat) override;
};

ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate::ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate(ZWPXwaylandKeyboardGrabManagerV1Interface *_q, Display *display)
    : QtWaylandServer::zwp_xwayland_keyboard_grab_manager_v1(*display, s_version)
    , q(_q)
{
}

void ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate::zwp_xwayland_keyboard_grab_manager_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate::zwp_xwayland_keyboard_grab_manager_v1_grab_keyboard(Resource *resource, uint32_t id, wl_resource *surface, wl_resource *seat)
{
    SurfaceInterface *s = SurfaceInterface::get(surface);
    if (!s) {
        // TODO: send error?
        qCWarning(KWAYLAND_SERVER) << "ServerSideDecorationInterface requested for non existing SurfaceInterface";
        return;
    }

    SeatInterface *st = SeatInterface::get(seat);
    if (!st) {
        // TODO: send error?
        qCWarning(KWAYLAND_SERVER) << "ServerSideDecorationInterface requested for non existing SeatInterface";
        return;
    }

    wl_resource *RbiResource = wl_resource_create(resource->client(), interface(), resource->version(), id);

    if (!RbiResource) {
        qCDebug(KWAYLAND_SERVER) << resource->client() << id;
        wl_client_post_no_memory(resource->client());
        return;
    }

    grab = new ZWPXwaylandKeyboardGrabV1Interface(q, s, st, RbiResource);

    QObject::connect(grab, &QObject::destroyed, q, [=]() {
        grab = nullptr;
        Q_EMIT q->zwpXwaylandKeyboardGrabV1Destroyed();
    });
    Q_EMIT q->zwpXwaylandKeyboardGrabV1Created(grab);
}

ZWPXwaylandKeyboardGrabManagerV1Interface::ZWPXwaylandKeyboardGrabManagerV1Interface(Display *display, QObject *parent)
    : QObject(parent)
    , d(new ZWPXwaylandKeyboardGrabManagerV1InterfacePrivate(this, display))
{
}

ZWPXwaylandKeyboardGrabManagerV1Interface::~ZWPXwaylandKeyboardGrabManagerV1Interface()
{
}

ZWPXwaylandKeyboardGrabV1Interface* ZWPXwaylandKeyboardGrabManagerV1Interface::getGrabClient()
{
    return d->grab;
}

/**
 * Provides the DBus service name and object path to a ZWPXwaylandKeyboardGrabV1 DBus interface.
 * This interface is attached to a wl_surface and provides access to where
 * the ZWPXwaylandKeyboardGrabV1 DBus interface is registered.
 * @since 5.42
 */
class KWAYLANDSERVER_EXPORT ZWPXwaylandKeyboardGrabV1InterfacePrivate : public QtWaylandServer::zwp_xwayland_keyboard_grab_v1
{

public:
    ZWPXwaylandKeyboardGrabV1InterfacePrivate(SurfaceInterface *s, SeatInterface *seat, wl_resource *resource);

    SurfaceInterface *surface;
    SeatInterface *seat;

protected:
    void zwp_xwayland_keyboard_grab_v1_destroy_resource(Resource *resource) override;

private:
    friend class ZWPXwaylandKeyboardGrabManagerV1Interface;

};

ZWPXwaylandKeyboardGrabV1InterfacePrivate::ZWPXwaylandKeyboardGrabV1InterfacePrivate(SurfaceInterface *s, SeatInterface *seat, wl_resource *resource)
    : QtWaylandServer::zwp_xwayland_keyboard_grab_v1(resource),
    surface(s),
    seat(seat)
{
}

void ZWPXwaylandKeyboardGrabV1InterfacePrivate::zwp_xwayland_keyboard_grab_v1_destroy_resource(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

ZWPXwaylandKeyboardGrabV1Interface::ZWPXwaylandKeyboardGrabV1Interface(ZWPXwaylandKeyboardGrabManagerV1Interface *parent, SurfaceInterface *s, SeatInterface *seat, wl_resource *resource)
    : QObject(parent)
    , d(new ZWPXwaylandKeyboardGrabV1InterfacePrivate(s, seat, resource))
{
}

ZWPXwaylandKeyboardGrabV1Interface::~ZWPXwaylandKeyboardGrabV1Interface()
{
}

SurfaceInterface* ZWPXwaylandKeyboardGrabV1Interface::surface() const
{
    return d->surface;
}

SeatInterface* ZWPXwaylandKeyboardGrabV1Interface::seat() const
{
    return d->seat;
}

}//namespace
