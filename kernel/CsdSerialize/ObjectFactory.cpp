//
// Created by yzqlwt on 2020/12/1.
//

#include "ObjectFactory.h"

#include <functional>



ObjectFactory::TInfo::TInfo(void)
        :_class("")
        ,_fun(nullptr)
        ,_func(nullptr)
{
}

ObjectFactory::TInfo::TInfo(const std::string& type, Instance ins)
        :_class(type)
        ,_fun(ins)
        ,_func(nullptr)
{
    ObjectFactory::getInstance()->registerType(*this);
}

ObjectFactory::TInfo::TInfo(const std::string& type, InstanceFunc ins)
        :_class(type)
        ,_fun(nullptr)
        ,_func(ins)
{
    ObjectFactory::getInstance()->registerType(*this);
}

ObjectFactory::TInfo::TInfo(const TInfo &t)
{
    _class = t._class;
    _fun = t._fun;
    _func = t._func;
}

ObjectFactory::TInfo::~TInfo(void)
{
    _class = "";
    _fun = nullptr;
    _func = nullptr;
}

ObjectFactory::TInfo& ObjectFactory::TInfo::operator= (const TInfo &t)
{
    _class = t._class;
    _fun = t._fun;
    _func = t._func;
    return *this;
}


ObjectFactory* ObjectFactory::_sharedFactory = nullptr;

ObjectFactory::ObjectFactory(void)
{

}

ObjectFactory::~ObjectFactory(void)
{
    _typeMap.clear();
}

ObjectFactory* ObjectFactory::getInstance()
{
    if ( nullptr == _sharedFactory)
    {
        _sharedFactory = new (std::nothrow) ObjectFactory();
    }
    return _sharedFactory;
}

void ObjectFactory::destroyInstance()
{
    delete _sharedFactory;
    _sharedFactory = nullptr;
}

NodeReaderProtocol* ObjectFactory::createObject(const std::string &name)
{
    NodeReaderProtocol *o = nullptr;
    do
    {
        const TInfo t = _typeMap[name];
        if (t._fun != nullptr)
        {
            o = t._fun();
        }else if (t._func != nullptr)
        {
            o = t._func();
        }
    } while (0);

    return o;
}

void ObjectFactory::registerType(const TInfo &t)
{
    _typeMap.emplace(t._class, t);
}