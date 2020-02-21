

#pragma once

#pragma warning(disable : 4996 4800)
typedef int Rank;

//位图Bitmap01Init类：以空间作为补偿，节省初始化时间（既允许插入，亦支持删除）
class Bitmap01Init
{
public:
    Bitmap01Init(Rank n = 8);//按指定（或默认）规模创建比特图（为测试暂时选用较小的默认值）
    ~Bitmap01Init();//析构时释放空间

    //接口
    void reset();//复位：从逻辑上切断所有校验环，O(1)
    void set(Rank k);//插入：从逻辑上将B[k]置为true，O(1)
    void clear(Rank k);//删除：从逻辑上将B[k]置为false，O(1)
    bool test(Rank k);//从逻辑上判断B[k]是否为true，O(1)

private:
    Rank _N;//规模_N
    Rank* _F;//规模为N的向量F，记录[k]被标记的次序（即其在栈T[]中的秩）

    Rank* _T;//容量为_N的栈_T，记录被标记各位秩的栈
    Rank _top;//栈顶指针_top
};