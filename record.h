#ifndef RECORD_H
#define RECORD_H


class Record
{
    public:
        Record(char* data, int sizeR, int col);
        virtual ~Record();
        char* toChar();
        int cantC, longitud;
        void initFromChar(char* data);
        void create();
        char* datav;
    protected:

    private:
};

#endif // RECORD_H
