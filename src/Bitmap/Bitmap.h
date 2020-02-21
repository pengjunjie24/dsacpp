

#pragma once

#pragma warning(disable : 4996 4800)

//位图
class Bitmap
{
public:
    Bitmap(int n = 8);//按指定或默认规模创建比特图
    Bitmap(char* file, int n = 8);//按指定或默认规模，从指定文件中读取比特图
    ~Bitmap();//析构时释放比特图空间

    void set(int k);
    void clear(int k);
    bool test(int k);

    void dump(char* file);//将位图整体导出至指定的文件，以便对此后的新位图批量初始化
    char* bits2string(int n);//将前n位转换为字符串
    void expand(int k);//若被访问的Bitmap[k]已出界，则需扩容
    void print(int n);//逐位打印以检验位图内容，非必需接口

protected:
    void init(int n);

private:
    char* _M;//比特图所存放的空间_M[]
    int _N;//容量为_N * sizeof(char) * 8比特
};