#include "hxmlmanager.h"

HXmlManager::HXmlManager(QObject *parent): QObject(parent)
{

    configurationFile=new QFile(QCoreApplication::applicationDirPath()+"/Ressources/files/homiss.config");
    configurationDom=new QDomDocument();

    connect(this,SIGNAL(fileNotOpened(QString)),this,SLOT(showError(QString)));
    reloadConfigDom();

}

HXmlManager::~HXmlManager()
{
    delete configurationDom;
    delete configurationFile;
}

QDomElement HXmlManager::findElement(QDomElement elem, const QString &tagName)
{
    QDomNode node= elem.firstChild();
    bool mached=false;
    QDomElement current;

    while(!node.isNull())
    {
        current=node.toElement();

        if (current.tagName()==tagName)
        {
            mached=true;
            break;
        }
        node=node.nextSibling();
    }

    if (mached)
    {
        return current;
    }
    else
    {
        qDebug() << tagName << "not found";
        return QDomElement();
    }
}

bool HXmlManager::modifAttr(QDomNode &mere, const QString &tagname, const QString &attrName, const QString &newValue)
{
    bool isValid=true;
    QDomElement mereElem;


    if (mere.isElement())
    {
        mereElem=mere.toElement();
    }
    else if (mere.isDocument())
    {
        mereElem=mere.toDocument().documentElement();
    }
    else
    {
        isValid=false;
    }



    if (isValid)
    {
        if (tagname=="self")
        {
            mereElem.setAttribute(attrName,newValue);
            return true;
        }
        else
        {
            QDomElement el= findElement(mereElem,tagname);

            if (!el.isNull())
            {
                el.setAttribute(attrName,newValue);
                return true;
            }
            else
            {
                qDebug() << tagname+" not  modified";
                return false;
            }
        }
    }
    else
    {
        qDebug() << "Type de node non pris en charge";
        return false;
    }

}

bool HXmlManager:: updateFile(QFile &file, const QString &content)
{
    if (!file.open(QIODevice::WriteOnly))
    {
       error();
       file.close();
       return false;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("utf-8"));
    out << content;
    file.close();
    return true;
}

bool HXmlManager:: updateConfigFIle()
{
    return updateFile(*configurationFile,configurationDom->toString());
}

bool HXmlManager::reloadDom(QFile &source, QDomDocument &target)
{
    bool res;

    if (!source.open(QIODevice::ReadOnly))
    {
        source.close();
        error();
        res=false;
    }

    if (target.setContent(&source))
    {
        res=true;
    }
    else
    {
        qDebug() << "DOMDocument not loaded";
       res=false;
    }

    source.close();
    return res;
}

bool HXmlManager::reloadConfigDom()
{
    return reloadDom(*configurationFile,*configurationDom);
}

void HXmlManager::error()
{
    emit fileNotOpened("Erreur d'ouverture du fichier de configuration!");
}

QDomDocument *HXmlManager::getConfigurationDom() const
{
    return configurationDom;
}

QSqlDatabase HXmlManager::getDataBase()
{
    QDomElement db= findElement(configurationDom->documentElement(),"database");
    QSqlDatabase res;

    if (!db.isNull())
    {
        res=*new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","DBFormXml"));
        res.setHostName(db.attribute("host"));
        res.setUserName(db.attribute("userName"));
        res.setPassword(db.attribute("pass"));
        res.setDatabaseName(db.attribute("DBName"));
        res.setPort(db.attribute("port").toInt());

        return res;
    }
    else
    {
        qDebug("Database from xml not instantied");
        return res;
    }
}

QString HXmlManager::getDatabaseParameter(DBInfoType type)
{
    QDomElement db= findElement(configurationDom->documentElement(),"database");

    if (!db.isNull())
    {
        switch (type)
        {
        case HOSTNAME: return db.attribute("host");
        case USERNAME: return db.attribute("userName");
        case PASSWORD: return  db.attribute("pass");
        case DBNAME: return  db.attribute("DBName");
        case PORT: return  db.attribute("port");
        }
    }
    else
    {
        qDebug("start parametre not mached");
        return QString();
    }
}

bool HXmlManager::getStartParameter(startParamaterType type)
{
    QString res;
    QDomElement start= findElement(configurationDom->documentElement(),"start");
    switch (type)
    {
    case GLOBAL: res=start.attribute("allDone");
        break;
    case SER: res=start.attribute("service");
        break;
    case FICHE: res=start.attribute("model");
        break;
    default: res=start.attribute("id");
        break;
    }

    if (res=="true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString HXmlManager::getAppId()
{
    return configurationDom->documentElement().attribute("id");
}

SERVICE* HXmlManager::getService()
{
    SERVICE *s= new SERVICE(findElement(configurationDom->documentElement(),"service").attribute("id").toInt(),findElement(configurationDom->documentElement(),"service").attribute("nom"),"CENHOSOA");
    return s;
}


bool HXmlManager::setDatabaseParameters(QString host, QString un, QString pass, QString DBName, QString port)
{
    reloadConfigDom();

    QDomElement el= findElement(configurationDom->documentElement(),"database");
    if(modifAttr(el,"self","host",host) && modifAttr(el,"self","userName",un) && modifAttr(el,"self","pass",pass) && modifAttr(el,"self","DBName",DBName) && modifAttr(el,"self","port",port))
    {
        updateConfigFIle();
        return true;
    }
    else
    {
        reloadConfigDom();
        return false;
    }

}

bool HXmlManager::setStartParameters(startParamaterType type, bool newVal)
{
    reloadConfigDom();
    QDomElement el= findElement(configurationDom->documentElement(),"start");
    bool res;
    QString nv;

    if (newVal)
        nv="true";
    else
        nv="false";

    switch (type)
    {
    case SER: res= modifAttr(el,"self","service",nv);
        break;
    case FICHE: res = modifAttr(el,"self","model",nv);
        break;
    case ID: res= modifAttr(el,"self","id",nv);
        break;
    default: res= modifAttr(el,"self","allDone",nv);
        break;
    }

    if (!getStartParameter(startParamaterType::FICHE) && !getStartParameter(startParamaterType::ID) && !getStartParameter(startParamaterType::SER))
    {
        modifAttr(el,"self","allDone","true");
        qDebug("all parameters are done");
    }

    if (!updateConfigFIle())
    {
        res=false;
        reloadConfigDom();
    }

    return res;

}

bool HXmlManager::setAppId(const QString &value)
{
    reloadConfigDom();
    bool res= modifAttr(*configurationDom,"self","id",value);
    res=reloadConfigDom();
    if(!res)
        reloadConfigDom();
    return res;
}

bool HXmlManager::setService(QString id, QString nom)
{
    reloadConfigDom();
    if (modifAttr(*configurationDom,"service","id",id) && modifAttr(*configurationDom,"service","nom",nom))
    {
        if (!updateConfigFIle())
        {
            reloadConfigDom();
        }
        return true;

    }
    else
    {
        return false;
    }
}

void HXmlManager::addUserConnected(QString id)
{
    reloadConfigDom();
    QDomElement newU=configurationDom->createElement("user");
    newU.setAttribute("connected","true");
    newU.setAttribute("id",id);
    newU.setAttribute("date",QDateTime::currentDateTime().toString("dddd dd/MMMM/yyyy hh:mm:ss"));

    QDomNode e=findElement(findElement(configurationDom->documentElement(),"connection"),"history").appendChild(newU);
    QString a= e.isNull()? "true": "false";
    qDebug() << "confirmation: "+a ;

    if (!updateConfigFIle())

    {
        reloadConfigDom();
    }

}

void HXmlManager::deconnectUser(QString id)
{
    reloadConfigDom();
    QDomElement h= findElement(findElement(configurationDom->documentElement(),"connection"),"history");
    QDomNode n=h.firstChild();
    bool found=false;

    while(!n.isNull())
    {
        QDomElement current= n.toElement();

        if (current.attribute("id")==id && current.attribute("connected")=="true")
        {
            current.setAttribute("connected","false");
            found=true;
        }
        n=n.nextSibling();
    }

    if (found)
    {
        if (!updateConfigFIle())
        {
            reloadConfigDom();
        }

    }

    else
    {
        qDebug() << "Aucune connection de l'utilisateur "+id+" existante";
    }


}

QDomElement HXmlManager::getConnectedUser()
{
    QDomElement h= findElement(findElement(configurationDom->documentElement(),"connection"),"history");
    QDomNode n=h.firstChild();

    while(!n.isNull())
    {
        QDomElement current= n.toElement();

        if (current.attribute("connected")=="true")
        {
            return current;
        }
        n=n.nextSibling();
    }
    return QDomElement();
}

QDomElement HXmlManager::getUser(QString id_user)
{
    QDomElement h= findElement(findElement(configurationDom->documentElement(),"connection"),"history");
    QDomNode n=h.firstChild();
    QDomElement *res= new QDomElement();

    while(!n.isNull())
    {
        QDomElement current= n.toElement();

        if (current.attribute("id")==id_user)
        {
            res= new QDomElement(current);
        }
        n=n.nextSibling();
    }
    return *res;
}

QString HXmlManager::getIdConnectedUser()
{
    QDomElement c= getConnectedUser();

    if (!c.isNull())
    {
        return c.attribute("id");
    }
    else
    {
        qDebug() <<  "no user connected";
        return QString();
    }
}

QString HXmlManager::getDateConnection(QString id_user)
{
    QDomElement res= getUser(id_user);
    if (!res.isNull())
    {
        return res.attribute("date");
    }
    else
    {
        return QString();
    }
}

QString HXmlManager::getUserConnectedConnexionDate()
{
    return getConnectedUser().attribute("date");
}

bool HXmlManager::isUserConnected()
{
    if (getConnectedUser().isNull())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QDomElement HXmlManager::addValue(QString id_menu, QString value, bool used,bool defaultVal)
{
    reloadConfigDom();

    if (!getValueOfMenu(id_menu,value).isNull())
    {
        qDebug() << "Value "+value+" existante dans "+id_menu;
        return QDomElement();
    }
    QDomElement menu = getMenu(id_menu,findElement(configurationDom->documentElement(),"model"));
    if (!menu.isNull())
    {
        QString usage= used? "true":"false";

        QDomElement ne= configurationDom->createElement("value");
        ne.setAttribute("content",value);
        ne.setAttribute("usage",usage);
        ne.setAttribute("default",defaultVal? "true":"false");

        menu.appendChild(ne);

        menu.setAttribute("nbVal",menu.attribute("nbVal").toInt()+1);
        if (!updateConfigFIle())
        {
            reloadConfigDom();
            return QDomElement();
        }

        return ne;
    }
    else
    {
        qDebug() << "Menu "+ id_menu+" not found pendant la recherche dans addValue";
        return QDomElement();
    }
}

QDomElement HXmlManager::addMenu(QString id, QString desc, QString mode, QList<QDomElement> values, QString id_supra, QString alias, bool ValueModifiable, bool parametrable, bool valueInverted,QDomNode ref,bool insertAfter)
{
    reloadConfigDom();

    QDomElement model=findElement(configurationDom->documentElement(),"model");

    if (!getMenu(id,model).isNull())
    {
        qDebug("Menu existante dans Add menu");
        emit menuExistante();
        return  QDomElement();
    }


    QDomElement n= configurationDom->createElement("menu");
    n.setAttribute("id",id);
    n.setAttribute("descriptif",desc);


    n.setAttribute("remplissage",mode);
    n.setAttribute("Alias",alias);
    n.setAttribute("nbVal",values.length());
    n.setAttribute("id_supra",id_supra);
    n.setAttribute("usage","true");
    n.setAttribute("valueModifiable",ValueModifiable? "true":"false");
    n.setAttribute("parametrable",parametrable? "true":"false");
    n.setAttribute("valueInverted",valueInverted? "true":"false");

    int i=0;
    while (i<values.length())
    {
        n.appendChild(values.at(i));
        i++;
    }
     QDomElement mere;

    if (id_supra.isEmpty())
    {
        mere=findElement(configurationDom->documentElement(),"model");
    }
    else
    {
        mere=getMenu(id_supra,model);

        if (mere.isNull())
        {
            qDebug() << "Menu not added, Menu super "+ id_supra+" not found";
            return  QDomElement();
        }

    }

    mere.appendChild(n);
//    if (ref.isNull())
//    {
//         mere.appendChild(n);
//    }
//    else
//    {
////        QDomNode node= mere.childNodes().;


//        if (insertAfter)
//        {
//            if (mere.insertAfter(n,ref).isNull())
//            {
////                QMessageBox::warning(nullptr,"error","une erreur s'est produite");
//            }
//        }
//        else
//        {
//            if (mere.insertBefore(n,ref).isNull())
//            {
////                QMessageBox::warning(nullptr,"error","une erreur s'est produite");
//            }
//        }
//    }




    if (!updateConfigFIle())
    {
        reloadConfigDom();
        return QDomElement();
    }

    return n;
}

QDomElement HXmlManager::getMenu(QString id,QDomElement container)
{

    QDomNode node=container.firstChild();
    QDomElement current;
    QDomElement res;

    while (!node.isNull())
    {
        current=node.toElement();
        if (current.tagName()=="menu")
        {

            if (current.attribute("id")==id)
            {
                res=current;
                break;
            }
            else
            {
                res=getMenu(id,current);
            }

            if (!res.isNull())
            {
                if (res.attribute("id")==id)
                {
                    break;
                }
            }
        }

        node=node.nextSibling();
    }

    return res;

}

QDomElement HXmlManager::updateMenu(QString id, QString desc, QString mode, bool usage, QString alias, QString nbVal,bool valueInverted)
{
    reloadConfigDom();
    QDomElement n= getMenu(id,findElement(configurationDom->documentElement(),"model"));

    if (n.isNull())
    {
        qDebug() << "Menu not updated, Menu "+id+" not found";
        return QDomElement();
    }

    n.setAttribute("id",id);
    n.setAttribute("descriptif",desc);
    n.setAttribute("remplissage",mode);
    n.setAttribute("Alias",alias);
    n.setAttribute("nbVal",nbVal);
    n.setAttribute("usage",usage? "true":"false");
    n.setAttribute("valueInverted",valueInverted? "true":"false");



    if (!updateConfigFIle())
    {
        reloadConfigDom();
        return QDomElement();
    }

    return QDomElement();
}

QDomElement HXmlManager::updateValue(QString id_menu,QString lastV, QString value, bool used,bool defaultVal)
{
    reloadConfigDom();
    QDomElement ne = getValueOfMenu(id_menu,lastV,true);


    if (!ne.isNull())
    {
        bool usg;
        if (ne.attribute("usage")=="true")
        {
            usg=true;
        }
        else
        {
            usg=false;
        }

        if (usg!=used)
        {
            QDomElement m= getMenu(id_menu,findElement(configurationDom->documentElement(),"model"));
            int nb= m.attribute("nbVal").toInt();
            if (used)
            {
                nb++;
            }
            else
            {
                nb--;
            }
            m.setAttribute("nbVal",nb);
        }


        QString usage= used? "true":"false";

        ne.setAttribute("content",value);
        ne.setAttribute("usage",usage);
        ne.setAttribute("default",defaultVal? "true":"false");

        if (!updateConfigFIle())
        {
            reloadConfigDom();
            return QDomElement();
        }

        return ne;
    }
    else
    {
        qDebug() << "Menu "+ id_menu+" not found";
        return QDomElement();
    }
}

QList<QDomElement> HXmlManager::addValues(QString id_menu, QList<QDomElement> values)
{
    QList<QDomElement> res;
    QDomElement menu=getMenu(id_menu,findElement(configurationDom->documentElement(),"model"));

    int nb= menu.attribute("nbVal").toInt();
    nb=nb+values.length();
    menu.setAttribute("nbVal",nb);

    if (menu.isNull())
    {
        return QList<QDomElement>();
    }

    int i=0;

    while (values.length())
    {
        if (values.at(i).tagName()=="value")
        {
            res.append(values.at(i));

            this->addValue(values.at(i).attribute("content"),values.at(i).attribute("usage"));
        }
        i++;
    }
    return res;
}

QList<QDomElement> HXmlManager::getSuperMenus()
{
    QList<QDomElement> res;

    QDomElement model= findElement(configurationDom->documentElement(),"model");

    QDomNode node= model.firstChild();
    while (!node.isNull())
    {
        QDomElement current= node.toElement();
        if (current.tagName()=="menu")
        {
            res.append(current);
        }
        node=node.nextSibling();
    }

    return res;
}

QList<QDomElement> HXmlManager::getMenuChilds(QString id)
{
    QList<QDomElement> res;
    QDomElement menu= getMenu(id,findElement(configurationDom->documentElement(),"model"));
    if (!menu.isNull())
    {
        QDomNode node= menu.firstChild();
        while (!node.isNull())
        {
            QDomElement current= node.toElement();
            if (current.tagName()=="menu")
            {
                res.append(current);
            }
            node=node.nextSibling();
        }
    }
    return  res;
}

QList<QDomElement> HXmlManager::getValuesOfMenu(QString id,bool withNull)
{
    QList<QDomElement> res;
    QDomElement menu= getMenu(id,findElement(configurationDom->documentElement(),"model"));
    if (!menu.isNull())
    {
        QDomNode node= menu.firstChild();
        while (!node.isNull())
        {
            QDomElement current= node.toElement();
            if (!withNull)
            {
                if (current.tagName()=="value" && current.attribute("usage")=="true")
                {
                    res.append(current);
                }
            }
            else
            {
                if (current.tagName()=="value")
                {
                    res.append(current);
                }
            }

            node=node.nextSibling();
        }
    }
    return  res;
}

QString HXmlManager::getDefaultValueOfMenu(QString id)
{
    QList<QDomElement> v= getValuesOfMenu(id);

    int i=0;

    while(i<v.length())
    {
        if (v.at(i).attribute("default")=="true")
        {
            return v.at(i).attribute("content");
        }
        i++;
    }

    qDebug("not found defautl value");
    return QString();
}

QDomElement HXmlManager::getValueOfMenu(QString id_menu, QString value, bool alsoNull)
{
    QList<QDomElement> v= getValuesOfMenu(id_menu,alsoNull);
    QDomElement res;

    int i=0;
    while (i<v.length())
    {
        if (v.at(i).attribute("content")==value)
        {
            res=v.at(i);
            break;
        }
        i++;
    }

    return  res;
}

QDomElement HXmlManager::setUsageValue(QString id_menu, QString content, bool usage)
{
    reloadConfigDom();
    QDomElement v= getValueOfMenu(id_menu,content);

    if (v.isNull())
    {
        return QDomElement();
    }

    v.setAttribute("usage",usage? "true" : "false");

    if (!updateConfigFIle())
    {
        reloadConfigDom();
        return QDomElement();
    }

    return v;

}

QDomElement HXmlManager::setUsageMenu(QString id, bool usage)
{
    reloadConfigDom();
    QDomElement m= getMenu(id,findElement(configurationDom->documentElement(),"model"));

    if (m.isNull())
    {
        return QDomElement();
    }

    m.setAttribute("usage",usage? "true": "false");

    QList<QDomElement> sous= getMenuChilds(id);
    QList<QDomElement> val= getValuesOfMenu(id);

    int i=0;

    while (i<sous.length())
    {
        setUsageMenu(sous.at(i).attribute("id"),usage);
        i++;
    }

    i=0;

    while (i<val.length())
    {
        setUsageValue(id,val.at(i).attribute("id"),usage);
        i++;
    }

    if (!updateConfigFIle())
    {
        reloadConfigDom();
        return  QDomElement();
    }
    return  m;
}

void HXmlManager::showError(QString msg)
{
    qDebug() << msg;
}
