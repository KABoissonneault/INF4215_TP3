#pragma once


namespace INF4215_TP3
{
    class QNode
    {
    public:

        QNode();

        float getQ();
        float getA();
        float getN();

        void setQ(float q);

    private:

        float Q;
        float A;
        float N;


    };
}

