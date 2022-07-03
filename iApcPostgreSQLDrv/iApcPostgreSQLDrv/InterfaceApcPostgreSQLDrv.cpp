#include "pch.h"
#include "InterfaceApcPostgreSQLDrv.h"
#include  "iApcPostgreSQLDrv.h"

InterfaceApcPostgreSQLDrv* InterfaceApcPostgreSQLDrv::createInstance()
{
    return new iApcPostgreSQLDrv();
}
//---------------------------------------------------------------------------

/*InterfaceApcPostgreSQLDrv* InterfaceApcPostgreSQLDrv::create()
{
    IApcFirebirdDrv* pFBDrv = nullptr;
    try
    {
        pFBDrv = IApcFirebirdDrv::createInstance();
    }
    catch (const std::bad_alloc&)
    {
        pFBDrv = nullptr;
    }
    catch (...)
    {
        pFBDrv = nullptr;
    }
    return pFBDrv;
};*/