#ifndef RUNTIMEMANGER_
#define RUNTIMEMANGER_


enum class statusCode {
    OK = 0,
    DEAD = 1,
    DEGRADED = 3,


};
class runtimeManger
{
    public:
        runtimeManger();
        virtual ~runtimeManger();

    protected:

    private:
};

#endif // RUNTIMEMANGER_
