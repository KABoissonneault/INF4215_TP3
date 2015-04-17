#include "QNode.h"


namespace INF4215_TP3
{
    QNode::QNode()
    {
        Q = 0;
        N = 0;
        A = 1;
    }

    float QNode::getQ()
    {
        return Q;
    }

    float QNode::getA()
    {
        return A;
    }

    float QNode::getN()
    {
        return N;
    }

    void QNode::setQ(float q)
    {
        Q = q;
        ++N;
        A *= 0.99f;
    }
}
