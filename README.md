vrcom
=====

C++ template library useful for exposing COM objects. It's LGPL based and it doesn't depend on ATL nor MFC.

Usage
=====

Back in 2000 when I decided to create this library there were two mainstream libraries for COM objects development to choose from: ATL or MFC.

MFC used the nested approach, where your object was turned into a COM object by the usage of some macros wich expanded to nested classes. These macros would eventually pull-in the IUnknown interface and the other interfaces you choose to implement.

ATL used the multiple inheritance, quicly leading to hard to deal with problems because of this.

That pushed me to create this library that took the both approaches: first, inherit your object from CComObject (defined in vcrObject.h), then add the interfaces you want to expose as nested classes (see vrcImpInterface.h).
