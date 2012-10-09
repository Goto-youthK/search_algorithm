#ifndef HASH_HPP
#define HASH_HPP

#include <cstdlib>
#include <cstring>
#include <string>
#include <list>
#include <vector>

using namespace std;
unsigned int hash_size = 101;   // ハッシュサイズ

/* キー値をハッシュ値に変換 */
const unsigned int convert(const string& str)
{
    int size = str.size();
    unsigned int h = 0;

    // ハッシュ関数
    for(int i = 0; i < size; i++) {
        h = h*size + (unsigned int)str[i];
    }

    /*
    // 素数の方が偏りが少なくなる
    if(size <= 9)
        hash_size = 101;
    else if(size <= 16)
        hash_size = 997;
    else if(size <= 25)
        hash_size = 10007;
    else if(size <= 36)
        hash_size = 100003;
    */

    // ハッシュ値
    return h % hash_size;
};

/* 連想配列クラス */
template <typename TKey, typename TValue>
class HASH {
public:
    // 型宣言
    typedef const unsigned int (*HashFunc)(const TKey&);
    typedef pair<const TKey, TValue> Pair;

protected:
    // クラス内用の型宣言
    typedef list<Pair> List;
    typedef typename List::iterator Iterator;
    typedef vector<List> Vector;

    // Vector要素の１つのListの検索結果と終了をセットにした構造体
    struct BeginEnd {
        typename Vector::size_type hval;// Vectorの要素位置
        Iterator found;                 // Listの検索結果
        Iterator end;                   // Listの終了
    };

private:
    Vector m_vec;
    HashFunc m_func;

public:
    // コンストラクタ
    HASH(HashFunc func = &convert)
        : m_vec(hash_size), m_func(func)
    {};

    // 値の取り出し
    TValue& operator [](const TKey& key) {
        BeginEnd result = _find(key);
        Iterator it = result.found;

        // キーが見つかった時
        if(it != result.end)
            return it->second;
        // キーが見つからなかった時
        else{
            it = addPair(key);// 新規追加
            return it->second;
        }
    };

    // キーの削除
    void remove(const TKey& key) {
        BeginEnd result = _find(key);
        // キーの検索結果
        Iterator it = result.found;

        // キーが見つからなかった時
        if(it == result.end)
            return;

        // Vectorの要素位置を取得
        m_vec[result.hval].erase(it);
    };

    // 検索
    Pair* find(const TKey& key) {
        BeginEnd result = _find(key);
        // キーの検索結果
        Iterator it = result.found;

        // キーが見つかった時
        if(it != result.end)
            return &*it;
        // キーが見つからなかった時
        else
            return NULL;
    };

    // 新規にキーと値のセットを追加します
    Iterator addPair(const TKey& key) {
        // Vectorの要素位置を取得
        const unsigned int hval = m_func(key);

        // セットの追加
        m_vec[hval].push_front(Pair(key, hval));

        return m_vec[hval].begin();
    };

protected:
    // マッピング値の検索
    BeginEnd _find(const TKey& key) {
        // Vectorの要素位置を取得
        const unsigned int hval = m_func(key);

        Iterator it;
        Iterator begin = m_vec[hval].begin();
        Iterator end = m_vec[hval].end();

        // 要素の中を1つずつ検索していく
        for(it = begin; it != end; it++){
            if(it->first == key) break;
        }

        // 要素位置の検索結果
        BeginEnd k = {hval, it, end};

        return k;
    };
};

#endif
