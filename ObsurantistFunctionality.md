# Introduction #

This tool is becoming my work tool and so functions are going in it which may not fit in the usual script tool, nor are they necessarily kosher.  But I need them, and in a sort of way, they are fitting for a script tool, since it involves managing the local computer itself.


# Details #

  * I need to purchase an external keyboard emulator, since for most of the tasks requiring a key pusher, a device layer is not going to work.  Software gets too smart to let passwords be pushed to it.  So, a physical device that takes a USB input and controls the mouse and keyboard can manage to simulate any login.
  * Can't really control login to computer this way since the software to control the external device won't be loaded.
  * Establish the VPN connection with the current password, including the certificate layer.  I've established in the past that all these windows do notify Windows when they present, but they do not accept spoofing (quite rational, really).  lister would catch the "Cannot contact tcp/ip layer" connect error, then check if vpn driver loaded, if not, start it, then spoof it via the external device.  Tada!  20 minutes saved just dealing with VPN drop out all day.
  * Trap annoying email popups like "all traffic monitored...." and auto close for me.
  * Open office communicator if not open, or reload after a VPN drop.  Verify connected.
  * Open outlook if not open.  Flush dumb emails that macros do not seem to do so well.
  * Assist with QC logins.  Essentially a connection that
    1. Opens Internet Explorer (QC plugin only available in IE)
> > 2) Waits interminably...
> > 3) Detects completion of component load
> > 4) Positions mouse over input field (can watch system mouse messages with Spy whilest moving mouse with ext spoofer.
> > 5) Pass in control information.
> > 6) Send enter key that triggers connection
> > 7) Wait and detect when/if grid opened.
  * Assist many other logins that are IE based.  Trap timeouts and reconnect.

Aint I a stinker?