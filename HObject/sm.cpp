#include "sm.h"



SM::SM(int id, QString label): LABELED (id,label)
{
}

SM::SM(const SM &other): LABELED(other)
{

}

SM::SM(): LABELED ()
{

}
