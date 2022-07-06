#include "pch.h"
#include "iApcPostgreSQLDrv.h"
#include  "tApcPostgreSQLDrv.h"

iApcPostgreSQLDrv* iApcPostgreSQLDrv::createInstance()
{
    return new tApcPostgreSQLDrv();
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