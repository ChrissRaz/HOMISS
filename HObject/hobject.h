
#ifndef HOBJECT_H
#define HOBJECT_H
#include <QObject>

namespace H
{
class HObject;
}

class HObject : public QObject
{
public:
    HObject(int id);

    HObject();
    ~HObject();

    int getId() const;
    void setId(int value);

    HObject &operator=(HObject &other);
    bool isNull() const;

private:
    void setNull(bool value);

protected:
    int m_id;
    bool null;
};

#endif // HOBJECT_H
