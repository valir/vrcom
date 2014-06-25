vrcom
=====

C++ template library useful for exposing COM objects. It's LGPL licenced and it doesn't depend on ATL nor MFC.
I started writing this library during my free time. When an internet startup hired me, I had the chance to polish them and then I used them into production environment.

Why another COM template library?
=================================

Well, this question is not that simple to anwser. Back in 1999 when I started 
VRCOM developpement, one could build COM objects by either using ATL or MFC. 
The two frameworks had a different approach. MFC used the nested classes approach 
while ATL used the mutliple inheritance approach.

The MFC approach is more close to COM components paradigm. But, you know, MFC 
is not that clean. In fact, MFC does not respect OOD rules. It's just a bunch 
of classes, heavily coupled, with plenty of global functions and variables. 
I know, you'll say it has been developped for real-world developpement not for 
school. Then, I'll say, what about it's overhead? The least COM object instantiation 
will bring in memory more than 2Mb of DLL code.

When MS realized the MFC problem, they developped ATL. The multiple inheritance 
style is very handy when one need to develop simple COM components. The same 
multiple-inheritance becomes a pain when one need to inherit a COM objet from 
another. Also, ATL lacks good documentation when coming to advanced tricks, 
like collection manipulation and OLE containers.

VRCOM tries to address these problems by combining multiple-inheritance and
nested class approach. Also, I'll do my best to provide you an outstanding documentation.

Ok, tell me more about it !
===========================

Have a look at the QuickStart example project, under the samples directory.

See also this blog entry:
http://www.rusu.info/wp/?p=287
