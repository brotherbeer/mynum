#include "mynum.h"
#include "myoperators.h"
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cassert>


#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

using namespace mynum;

void test_construct();
void test_assign();
void test_copy();
void test_string_conversion();
void test_abs();
void test_neg();
void test_zero();
void test_cmp();
void test_mul();
void test_sqr();
void test_kmul();
void test_ksqr();
void test_add();
void test_sub();
void test_div();
void test_mod();
void test_and();
void test_or();
void test_xor();
void test_not();
void test_shl();
void test_shr();
void test_pow();
void test_pom();
void test_bits();
void test_property();
void test_swap();
void test_operators();
void test_string_assignment();
void test_string_reserve();
void test_string_insert();
void test_string_remove();
void test_string_strip();
void test_string_find();
void test_string_load();
void test_string_dump();
void test_string();
void test_add_small();
void test_sub_small();
void test_mul_small();
void test_div_small();
void test_mod_small();
void test_and_small();
void test_or_small();
void test_xor_small();
void test_basic_type_conversion();
void test_prime();

void test_detail()
{
    srand((unsigned int)time(NULL));

    test_construct();
    test_assign();
    test_copy();
    test_string_conversion();
    test_abs();
    test_neg();
    test_zero();
    test_cmp();
    test_property();
    test_swap();
    test_string();
    test_add();
    test_sub();
    test_mul();
    test_kmul();
    test_sqr();
    test_ksqr();
    test_div();
    test_mod();
    test_pow();
    test_pom();
    test_and();
    test_or();
    test_xor();
    test_not();
    test_shl();
    test_shr();
    test_bits();
    test_add_small();
    test_sub_small();
    test_mul_small();
    test_div_small();
    test_mod_small();
    test_and_small();
    test_or_small();
    test_xor_small();
    test_basic_type_conversion();
    test_prime();
}

int main(int argc, char* argv[])
{
    assert(min_base() == 2);
    assert(max_base() == 36);

    if (argc > 1)
    {
        int times;
        char confirm;
        std::stringstream ss(argv[1]);
        ss >> times;
        if (times > 0)
        {
            std::cout << ("ready?[y/n]");
            std::cin >> confirm;
            if (confirm == 'y')
            {
                for (int i = 0; i < times; i++)
                {
                    test_detail();
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            std::cout << "an integer greater than 0 required" << std::endl;
        }
    }
    else
    {
        test_detail();
    }
    std::cout << "common test OK!" << std::endl;
    return 0;
}

typedef number_t NN;

bool chance(int n);
void random(NN& a);
void create_big(NN& x, int size);

void test_construct()
{
    {
        NN a(NULL), b, c, d(0);
        assert(eq(a, b));
        assert(eq(c, d));
        assert(eq(a, c));
        assert(eq(a, d));
        NN e(""), f("0"), g("-0"), h("-0", 16);
        assert(eq(e, f));
        assert(eq(a, e));
        assert(eq(b, e));
        assert(eq(b, h));
        a = b; assert(eq(a, b));
        c = e; assert(eq(c, e));
        e = f; assert(eq(e, f));
    }{
        NN a("123456789123456789123456789");
        NN b(123456789);
        NN c((long)12);
        NN d((long long)12);
        assert(neq(a, b));
        assert(a(10) == "123456789123456789123456789");
        assert(b.to_string() == "123456789");
        assert(c.to_string() == "12");
        assert(d.to_string() == "12");
    }{
        NN a("123456789"), b(123456789); assert(eq(a, b));
    }{
        NN a("10ff0023457ABCD", 16), b("76543610947349453"); assert(eq(a, b));
        NN c("-10ff0023457ABCD", 16), d("-76543610947349453"); assert(eq(c, d));
    }{
        NN a("", 16);         assert(a.is_zero());
        NN b(NULL, 16);       assert(b.is_zero());
        NN c((char*)NULL, 8); assert(c.is_zero());
        NN d("");             assert(d.is_zero());
        NN e((char*)NULL);    assert(e.is_zero());
        NN f("0", 10);        assert(f.is_zero());
        NN g("0000", 16);     assert(g.is_zero());
        NN h("00000", 13);    assert(h.is_zero());
        NN i("00000000", 16); assert(i.is_zero());
        NN j("00000000000000", 19); assert(j.is_zero());
    }{
        NN a("a", 16);         assert(eq(a, 0xa));
        NN b("ab", 16);        assert(eq(b, 0xab));
        NN c("abc", 16);       assert(eq(c, 0xabc));
        NN d("abcd", 16);      assert(eq(d, 0xabcd));
        NN e("abcde", 16);     assert(eq(e, 0xabcde));
        NN f("abcdef", 16);    assert(eq(f, 0xabcdef));
        NN g("abcdef1", 16);   assert(eq(g, 0xabcdef1));
        NN h("abcdef12", 16);  assert(eq(h, 0xabcdef12));
    }{
        NN a("0", 2), b("1", 2), c("11", 2), d("111", 2);
        assert(eq(a, 0));
        assert(eq(b, 1));
        assert(eq(c, 3));
        assert(eq(d, 7));
        NN e("100010101011011011010010111111111001000101010001110101101011001111010100011111001001101", 2);
        assert(eq(e, NN("83847563019273457210375757")));
        NN f("00100010101011011011010010111111111001000101010001110101101011001111010100011111001001101", 2);
        assert(eq(f, NN("83847563019273457210375757")));
        NN g("11111111111111111111111111111111", 2);
        assert(eq(g, 0xffffffff));
        NN h("1111111111111111", 2);
        assert(eq(h, 0xffff));
    }{
        NN a("0", 8), b("1", 8), c("8", 10);
        assert(a.to_oct_string() == "0");
        assert(b.to_oct_string() == "1");
        assert(c.to_oct_string() == "10");
        NN e("1243214567654322345667625253635315253647434553212212344", 8);
        assert(eq(e, NN("7710276014938769938930450760400327405668642919652")));
    }{
        NN a("ffffabcdabcdeeee", 16), b(0xffffabcdabcdeeeeULL);
        assert(eq(a, b));
    }{
        NN a("1234567890"); a.release(); assert(a.is_zero());
        NN b; b.release(); assert(b.is_zero());
    }{
        NN a("2132314432123432323234", 5);  assert(eq(a, NN("1116483718636069")));
        NN b("2132314432123432323234", 6);  assert(eq(b, NN("49617098196310822")));
        NN c("2132314432123432323234", 7);  assert(eq(c, NN("1235090655865747923")));
        NN d("2132314432123432323234", 9);  assert(eq(d, NN("235401196012841910865")));
        NN e("2132314432123432323234", 32); assert(eq(e, NN("82518724560440499616058857064548")));
        NN f("2132314432123432323234", 33); assert(eq(f, NN("157381945262979943192432190607001")));
        NN g("2132314432123432323234", 36); assert(eq(g, NN("976962556466868338958243803839312")));
        NN h("xxxxxxxx4j6473829202108172636454fgh64i74373823893293094058574c65251a4153648596069587473463535465757", 34);
        assert(eq(h, NN("41344028102870176563305997529441614984333017185992689784270345138316739022495793606262711909389364726981774832025688250243376430885399855868948295890581")));
        NN i("xxxxxxxx4j6473829202108172636454fgh64i74373823893293094058574c65251a4153648596069587473463535465757", 35);
        assert(eq(i, NN("707573262359867469722108287531813156522232983532435655888961385815574840755970285498631371438854997683935219937026688181345809812572009834620486153142507")));
        NN j("xxxxxxxx4j6473829202108172636454fgh64i74373823893293094058574c65251a4153648596069587473463535465757", 36);
        assert(eq(j, NN("11178755861740141374843999603430165213965899583171658065043666848036688188574509754493515917379914805991183712992559640751067510545467802062207656437684587")));
    }{
        NN a("", 2), b("", 16), c("8812", 2), d("9999", 8), e("abcd", 10), f("xyz", 16), g("123\xff\xf0\xe0\x00\xab", 2), h("578298234\xff\xf0\xe0\x00\xab", 8), i("\xffxxx\xf0\xe0\x00\xab", 10), j("qqqqqqq\xff\xf0\xe0\x00\xab", 16);
        NN k("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff", 256, 2);
        NN l("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff", 256, 8);
        NN m("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff", 256, 10);
        NN n("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff", 256, 16);
    }{
        string_t s1("123123"), s2;
        NN a(s1), b(s2), c(s1, 0, 0, 10), d(s1, s1.len, s1.len, 12);
        assert(a == 123123); assert(b == 0);
        assert(c == 0); assert(d == 0);
        NN e(s1, 0, 3, 12); assert(e == 171);
        NN f(s1, 0, s1.len, 12); assert(f == 295659);
        NN g(s1, 100, s1.len, 12); assert(g == 0);
        NN h(s1, 100, 1000, 12); assert(h == 0);
        NN i(s1, 3, 1000, 12); assert(i == 171);
    }{
        const char* s = "12345";
        NN a(s, 0, 10), b(s, 1, 12), c(s, 5, 16);
        assert(a == 0); assert(b == 1); assert(c == 0x12345);
        s = NULL;
        NN d(s, 0, 10), e(s, 1, 12), f(s, 5, 16), g(s, 0, 13);
        assert(d == 0); assert(e == 0); assert(f == 0); assert(g == 0);
        NN h(s, 10), i(s, 12), j(s, 8);
        assert(h == 0); assert(i == 0); assert(j == 0);
        s = "";
        NN k(s, 10), l(s, 12), m(s, 39);
        assert(k == 0); assert(l == 0); assert(m == 0);
    }{
        NN a("-", 2); assert(a == 0);
        NN b("-", 16); assert(b == 0);
        NN c("-", 10); assert(c == 0);
        NN d("-", 11); assert(d == 0);
        NN e("-", 36); assert(e == 0);
    }{
        NN a; load(a, "101 0110101010101111 1110101010101010 1010101010111111 1111000111000001", 2);
        NN b(a, 0, 1); assert(b == 1);
        NN c(a, 0, 2); assert(c == 1);
        NN d(a, 2, 4); assert(d == 0);
        NN e(a, 0, 36); assert(e == NN("101010101010101111111111000111000001", 2));
        NN f(a, 6, 9); assert(f == 7);
        NN g(a, 16, 48); assert(g == NN("11101010101010101010101010111111", 2));
        NN h(a, 45, a.bits_count()); assert(h == NN("1010110101010101111111", 2));
        NN i(a, 0, a.bits_count()); assert(i == a);
        NN j(a, 49, 64); assert(j == NN("11010101010111", 2));
        NN k(a, 8, 60); assert(k == NN("1010101011111110101010101010101010101011111111110001", 2));
        NN l(a, 8, 8); assert(!l);
        NN m(a, 9, 8); assert(!m);
    }{
        NN O; load(O, "11101101 0110101010101111 0000000000000000 0000000000000000 0000000000000000", 2);
        NN a(O, 0, 7); assert(a == 0);
        NN b(O, 17, 18); assert(b == 0);
        NN c(O, 48, 49); assert(c.is_po2());
        NN d(O, 48, 50); assert(d == 3);
        NN e(O, 0, 50); assert(e == 844424930131968ULL);
        NN f(O, 48, 53); assert(f == 0xf);
    }
}

void test_assign()
{
    {
        NN a;
        a.bits_reserve(1000);
#if UNITBITS == 16
        assert(a.cap == 64);
#elif UNITBITS == 32
        assert(a.cap == 32);
#endif
        a = char(123); assert(a(10) == "123");
        a = short(123); assert(a(10) == "123");
        a = int(123); assert(a(10) == "123");
        a = long(123); assert(a(10) == "123");
        a = (long long)(123); assert(a(10) == "123");
        a = short(-123); assert(a(10) == "-123");
        a = int(-123); assert(a(10) == "-123");
        a = long(-123); assert(a(10) == "-123");
        a = (long long)(-123); assert(a(10) == "-123");
        a = (unsigned short)(123); assert(a(10) == "123");
        a = (unsigned int)(123); assert(a(10) == "123");
        a = (unsigned long)(123); assert(a(10) == "123");
        a = (unsigned long long)(123); assert(a(10) == "123");
    }{
        NN a;
        a = char(123); assert(a(10) == "123");
        a = short(123); assert(a(10) == "123");
        a = int(123); assert(a(10) == "123");
        a = long(123); assert(a(10) == "123");
        a = (long long)(123); assert(a(10) == "123");
        a = short(-123); assert(a(10) == "-123");
        a = int(-123); assert(a(10) == "-123");
        a = long(-123); assert(a(10) == "-123");
        a = (long long)(-123); assert(a(10) == "-123");
        a = (unsigned char)(123); assert(a(10) == "123");
        a = (unsigned short)(123); assert(a(10) == "123");
        a = (unsigned int)(123); assert(a(10) == "123");
        a = (unsigned long)(123); assert(a(10) == "123");
        a = (unsigned long long)(123); assert(a(10) == "123");
    }{
        NN a; a = 0xabcdffffffffabcdULL; assert(a(16) == "abcdffffffffabcd");
        NN b; b = 0xabcdffffU; assert(b(16) == "abcdffff");
        NN c; c = (unsigned short)(0xabcd); assert(c(16) == "abcd");
    }{
        NN a("12345678901234567890");
        a.bits_reserve(1000);
        assert(a(10) == "12345678901234567890");
#if UNITBITS == 16
        assert(a.cap == 64);
#elif UNITBITS == 32
        assert(a.cap == 32);
#endif
    }{
        NN a(123), b(456), c, d(111), e;
        a.assign("11111111111010101101010", 2);  assert(a.to_string(2) == "11111111111010101101010");
        b.assign("12312343453465651556567", 8);  assert(b.to_string(8) == "12312343453465651556567");
        c.assign("23546252353252352353325", 10); assert(c.to_string(10) == "23546252353252352353325");
        d.assign("bdeeee454563aaa43534562", 16); assert(d.to_string(16) == "bdeeee454563aaa43534562");
        e.assign("23412412412421421412421", 17); assert(e.to_string(17) == "23412412412421421412421");
        a.assign("bdeeee454563aaa43534562", 20); assert(a.to_string(20) == "bdeeee454563aaa43534562");
        a.assign("bdeeee454563aaa43534562", 21); assert(a.to_string(21) == "bdeeee454563aaa43534562");
        a.assign("bdeeee454563aaa43534562", 22); assert(a.to_string(22) == "bdeeee454563aaa43534562");
    }{
        NN a(12345), b, c;
        b.assign(a);
        assert(a.assign(a) == a);
        assert(a.assign(a) == b);
        assert(12345 == b);
        c.bits_reserve(100);
        slen_t cap0 = c.cap;
        c.assign(a);
        slen_t cap1 = c.cap;
        assert(c == a);
        assert(cap0 == cap1);
        b.release();
        a.assign(b);
        assert(a == 0);
    }{
        NN a("1111111111111111"), b;
        a.assign(""); assert(a == 0);
        a.assign("1234567890", 3, 10); assert(a == 123);
        a.assign((char*)NULL); assert(a == 0);
        a.assign("1234567890"); assert(a == 1234567890);
        a.assign(NULL); assert(a == 0);
        a.assign((char*)NULL); assert(a == 0);
    }{
        NN a(123), b;
        a.assign("123123", 0); assert(a == 0);
        a.assign("123123", 1); assert(a == 0);
        a.assign("123123", 37); assert(a == 0);
        b.assign("123123", 0); assert(b == 0);
        b.assign("123123", 1); assert(b == 0);
        b.assign("123123", 37); assert(b == 0);
    }{
        NN a("675757575575757575"), b(123), c, d, e, f, g, h, i, j, k, l;
        const char* p = "123123";
        a.assign(p); assert(a == 123123);
        b.assign((char*)NULL); assert(b == 0);
        c.assign(p, 3, 12); assert(c == 171);
        d.assign(p, 0, 10); assert(d == 0);
        e.assign(p, 0, 16); assert(e == 0);
        f.assign(p, 0, 19); assert(f == 0);
        p = NULL;
        g.assign(p); assert(g == 0);
        h.assign(p, 2); assert(h == 0);
        i.assign(p, 1111, 13); assert(i == 0);
        p = "";
        j.assign(p); assert(j == 0);
        k.assign(p, 8); assert(k == 0);
        l.assign(p, 0, 16); assert(l == 0);
    }{
        string_t s1("123123"), s2;
        NN a, b, c, d, e, f, g, h, i;
        a.assign(s1); b.assign(s2); c.assign(s1, 0, 0, 10); d.assign(s1, s1.len, s1.len, 12);
        assert(a == 123123); assert(b == 0);
        assert(c == 0); assert(d == 0);
        e.assign(s1, 0, 3, 12); assert(e == 171);
        f.assign(s1, 0, s1.len, 12); assert(f == 295659);
        g.assign(s1, 100, s1.len, 12); assert(g == 0);
        h.assign(s1, 100, 1000, 12); assert(h == 0);
        i.assign(s1, 3, 1000, 12); assert(i == 171);
    }{
        NN a, b, c, d, e;
        a.assign("-", 2); assert(a == 0);
        b.assign("-", 16); assert(b == 0);
        c.assign("-", 10); assert(c == 0);
        d.assign("-", 11); assert(d == 0);
        e.assign("-", 36); assert(e == 0);
    }{
        NN a, b, c, d;
        load(a, "101 0110101010101111 1110101010101010 1010101010111111 1111000111000001", 2);
        b.assign(a, 0, 1); assert(b == 1);
        b.assign(a, 2, 3); assert(b == 0);
        b.assign(a, 6, 9); assert(b == 7);
        c.assign(a, 16, 48); assert(c == 3937053375U);
        d.assign(a, 45, a.bits_count()); assert(d == 2839935);
        d.assign(a, 32, 33); assert(d == 0);
        d.assign(a, 33, 34); assert(d == 1);
        d.assign(a, 49, 64); assert(d == NN("11010101010111", 2));
        d.assign(a, a.bits_count() - 1, a.bits_count()); assert(d == 1);
        d.assign(a, 0, 0); assert(d == 0);
        d.assign(a, 1, 0); assert(d == 0);

        NN O; load(O, "11101101 0110101010101111 1110101010101010 1010101010111111 1111000111000001", 2);
        a.assign(O); a.assign(a, 0, 1); assert(a == 1);
        a.assign(O); a.assign(a, 2, 3); assert(a == 0);
        a.assign(O); a.assign(a, 64, 67); assert(a == NN("101", 2));
        a.assign(O); a.assign(a, 32, 48); assert(a == NN("1110101010101010", 2));
        a.assign(O); a.assign(a, 32, 49); assert(a == NN("11110101010101010", 2));
        a.assign(O); a.assign(a, 32, 50); assert(a == NN("111110101010101010", 2));
        a.assign(O); a.assign(a, 32, 51); assert(a == NN("1111110101010101010", 2));
        a.assign(O); a.assign(a, 32, 52); assert(a == NN("11111110101010101010", 2));
        a.assign(O); a.assign(a, 32, 53); assert(a == NN("11111110101010101010", 2));
        a.assign(O); a.assign(a, 49, 64); assert(a == NN("11010101010111", 2));
        a.assign(O); a.assign(a, 7, 62); assert(a == NN("1010101010111111101010101010101010101010111111111100011", 2));

        load(O, "11101101 0110101010101111 0000000000000000 0000000000000000 0000000000000000", 2);
        a.assign(O); a.assign(a, 17, 18); assert(a == 0);
        a.assign(O); a.assign(a, 48, 49); assert(a.is_po2());
        a.assign(O);
        a.assign(a, 48, 50); assert(a == 3);
        a.assign(O); a.assign(a, 0, 50); assert(a == 844424930131968ULL);
        a.assign(O); a.assign(a, 48, 53); assert(a == 0xf);
        a.assign(O); a.assign(a, 0, a.bits_count()); assert(a == O);
        a.assign(O); a.assign(a, 32, 48); assert(a == 0);
        a.assign(O); a.assign(a, 48, 49); assert(a == 1);
        a.assign(O); a.assign(a, 32, 64); assert(a == NN("01101010101011110000000000000000", 2));
        a.assign(O); a.assign(a, 48, 64); assert(a == NN("0110101010101111", 2));
        a.assign(O); a.assign(a, 49, 64); assert(a == NN("11010101010111", 2));
        a.assign(O); a.assign(a, 14, 71); assert(a == NN("110110101101010101011110000000000000000000000000000000000", 2));
        a.assign(O); a.assign(a, 14, 14); assert(a == 0);
        a.assign(O); a.assign(a, 15, 14); assert(a == 0);
    }
}

void test_copy()
{
    {
        NN a("742354345232453423678231459849423"), b;
        b.copy(b);
        a.copy(a);
        assert(b.is_zero());
        assert(eq(a, NN("742354345232453423678231459849423")));
        b.copy(a); assert(eq(b, a));
        a.copy(b); assert(eq(b, a));
        b.release();
        a.copy(b);
        assert(eq(b, a));
        assert(a.is_zero());
        assert(a.dat == NULL);
        assert(a.len == 0);
        assert(a.cap == 0);
    }
    {
        NN a(12345), b, c;
        b.copy(a);
        assert(b == a);
        assert(b.cap == a.cap);
        c.bits_reserve(100);
        c.copy(a);
        assert(c == a);
        assert(c.cap == a.cap);
    }
}

void test_abs()
{
    {
        NN a, res;
        abs(a, res);
        assert(eq(res, 0));
        res = abs(a);
        assert(eq(res, 0));
        set_abs(a);
        assert(a.is_zero());
    }
    {
        NN a(-3), res;
        abs(a, res);
        assert(eq(res, 3));
        res = abs(a);
        assert(eq(res, 3));
        set_abs(a);
        assert(eq(a, 3));
    }
    {
        NN a("77777777777777777777", 8);
        NN b("-77777777777777777777", 8);
        NN res;
        abs(a, res);
        assert(cmp(a, res) == 0);
        assert(cmp(res, b) != 0);
        abs(b, res);
        assert(cmp(b, res) != 0);
        assert(cmp(res, a) == 0);
        res = abs(a);
        assert(cmp(a, res) == 0);
        res = abs(b);
        assert(cmp(res, a) == 0);
    }
    {
        NN a("-1234567890000000000000");
        NN b = a.abs();
        assert(cmp(a, NN("-1234567890000000000000")) == 0);
        assert(cmp(b, NN("1234567890000000000000")) == 0);
        abs(a, a);
        assert(cmp(a, NN("1234567890000000000000")) == 0);
    }
    {
        NN a("-1234567890000000000000");
        a.set_abs();
        assert(cmp(a, NN("1234567890000000000000")) == 0);
        NN b("-1234567890000000000000");
        set_abs(b);
        assert(cmp(b, NN("1234567890000000000000")) == 0);
    }
}

void test_neg()
{
    {
        NN a, b(-3), c("3142341241234123412412"), res;
        assert(eq(a.neg(), 0));
        assert(eq(b.neg(), 3));
        assert(eq(c.neg(), NN("-3142341241234123412412")));
        assert(eq(neg(c), NN("-3142341241234123412412")));
        neg(c, res);
        assert(eq(res, NN("-3142341241234123412412")));
    }
    {
        NN a, b(-3), c("3142341241234123412412"), res;
        assert(eq(set_neg(a), 0));
        assert(eq(set_neg(b), 3));
        set_neg(c);
        assert(eq(c, NN("-3142341241234123412412")));
    }
}

void test_zero()
{
    NN a, b(0), c(0L), d(0UL), e(0ULL);
    assert(a.is_zero());
    assert(b.is_zero());
    assert(c.is_zero());
    assert(d.is_zero());
    assert(e.is_zero());
 
    NN z;
    add(a, b, z);
    assert(z.is_zero());
    sub(a, b, z);
    assert(z.is_zero());
    mul(a, b, z);
    assert(z.is_zero());
 
    NN f("1111111111111111111111111111");
    mul(f, b, z);
    assert(z.is_zero());
    sub(f, f, z);
    assert(z.is_zero());

    NN x0("00000000", 2);
    assert(x0.is_zero());
    NN x1("00000000", 8);
    assert(x1.is_zero());
    NN x2("00000000", 10);
    assert(x2.is_zero());
    NN x3("00000000", 16);
    assert(x3.is_zero());
}

void test_string_conversion()
{
    {
        NN a;
        assert(a.to_bin_string() == "0");
        assert(a.to_dec_string() == "0");
        assert(a.to_hex_string() == "0");
        NN b("a", 16);
        assert(b.to_bin_string() == "1010");
        assert(b.to_bin_string() == "1010");
        assert(b.to_oct_string() == "12");
        assert(b.to_oct_string() == "12");
        assert(b.to_dec_string() == "10");
        assert(b.to_dec_string() == "10");
        assert(b.to_hex_string() == "a");
        assert(b.to_hex_string() == "a");
        NN c("ab", 16);
        assert(c.to_bin_string() == "10101011");
        assert(c.to_dec_string() == "171");
        assert(c.to_hex_string() == "ab");
        NN d("3ab", 16);
        assert(d.to_bin_string() == "1110101011");
        assert(d.to_dec_string() == "939");
        assert(d.to_hex_string() == "3ab");
        NN e("abcdef", 16);
        assert(e.to_hex_string() == "abcdef");
        NN f("00000000000", 16);
        assert(f.to_bin_string() == "0");
        assert(f.to_hex_string() == "0");
        assert(f.to_dec_string() == "0");
        NN g("00000000001", 16);
        assert(g.to_bin_string() == "1");
        assert(g.to_hex_string() == "1");
        assert(g.to_dec_string() == "1");
    }{
        NN b("111100001111");
        assert(b.to_bin_string() == "1100111011110000100110111101101010111");
        assert(b.to_hex_string() == "19de137b57");
        assert(b.to_dec_string() == "111100001111");
        NN c("011100001111");
        assert(c.to_hex_string() == "2959c9357");
        assert(c.to_dec_string() == "11100001111");
        NN d("001100001111");
        assert(d.to_hex_string() == "4190af57");
        assert(d.to_dec_string() == "1100001111");
        NN e("000100001111");
        assert(e.to_hex_string() == "5f5e557");
        assert(e.to_dec_string() == "100001111");
        NN f("000000001111");
        assert(f.to_hex_string() == "457");
        assert(f.to_dec_string() == "1111");
        NN g("100000000");
        assert(g.to_hex_string() == "5f5e100");
        assert(g.to_dec_string() == "100000000");
    }{
        NN a("1735ef68364257746471392170cb6cede21eac2d9c09250530253d8e19822401f94f196428ba28db55cf993b", 16);
        assert(a.to_hex_string() == "1735ef68364257746471392170cb6cede21eac2d9c09250530253d8e19822401f94f196428ba28db55cf993b");
        assert(a.to_dec_string() == "831776130406075456318059131933147606881331638817291364651526616059368720837846621607085626475132801227067");
    }{
        NN a("-f5cded75ce69b9a5febea72da2a0deeca11d6461bb1bba97acdb8ec84f1de343d43d5e579f2dd38dcef094d23", 16);
        assert(a.to_hex_string() == "-f5cded75ce69b9a5febea72da2a0deeca11d6461bb1bba97acdb8ec84f1de343d43d5e579f2dd38dcef094d23");
        assert(a.to_dec_string() == "-140938015441349122275607744040100968451721604618546777675101779553041889157401398239202274198932198680775971");
        NN b("-1", 16);
        assert(b.to_hex_string() == "-1");
        assert(b.to_dec_string() == "-1");
    }{
        NN z, a(1), b(2), c(65535);
        assert(z.to_bin_string() == "0");
        assert(a.to_bin_string() == "1");
        assert(b.to_bin_string() == "10");
        assert(c.to_bin_string() == "1111111111111111");
        NN d("947565783920385712374650102837492015666");
        assert(d.to_bin_string() == "1011001000110111101000100000100000111101010101001010000011110111100001001010101001111010101101110001110010010001110110001000110010");
        NN e("999999999999999999");
        assert(e.to_bin_string() == "110111100000101101101011001110100111011000111111111111111111");
        NN f(0x3abcd);
        assert(f.to_bin_string() == "111010101111001101");
        NN g("3abcdabcd", 16);
        assert(g.to_bin_string() == "1110101011110011011010101111001101");
        NN h("-3abcdabcd", 16);
        assert(h.to_bin_string() == "-1110101011110011011010101111001101");
    }{
        NN a("0"), b("1"), c("8"), d("9"), e("7710276014938769938930450760400327405668642919652"), f("-7710276014938769938930450760400327405668642919652");
        NN g("862983012983012983102948230498230472398457349857397429734298742472348230582038502345834985739487529873492873429837465298645928376512313141241029410948157896573846538798");

        assert(a.to_oct_string() == "0");
        assert(b.to_oct_string() == "1");
        assert(c.to_oct_string() == "10");
        assert(d.to_oct_string() == "11");
        assert(e.to_oct_string() == "1243214567654322345667625253635315253647434553212212344");
        assert(f.to_oct_string() == "-1243214567654322345667625253635315253647434553212212344");
        assert(g.to_oct_string() == "724237302571245323267024302143074364437171474117553576354644237637437655304037464112373605465151764431245054727550650520332635015105217246230502034676140350142504067345317274407273745056");
    }{
        NN a("-24126638821528938662435"), b("13357395038215"), c("1371994103959468758");
        NN d("16953102612315404"), e("361152674620498546"), f("5128268713369876810"), g("53372981233918129160");
        NN h("22818236586427407109557319585840701736614699");
        NN i("-215796735136656350604792165440915");
        NN j("15072046441264844726899206764135524234100507025931766");
        NN n("30487158740090870047953397897551091142555");
        NN o("-67815956182242799070103920387792872331841");
        NN z("111081904039280882364734303563924724896151377915077421802106532333963046922011197865487511669501041505270875");

        assert(a.to_string(3) == "-22011111102211211121111000000111101010101101112");
        assert(b.to_string(4) == "3002120002302111000013");
        assert(c.to_string(5) == "43002120002344402111000013");
        assert(d.to_string(6) == "434532123453512234512");
        assert(e.to_string(7) == "434532123453512234512");
        assert(f.to_string(8) == "434532123453512234512");
        assert(g.to_string(9) == "434532123453512234512");
        assert(h.to_string(11) == "464647382827a36557a171828394a9000595767650");
        assert(i.to_string(12) == "-aaaaabbb127363647838bb1b2b3b2b");
        assert(j.to_string(19) == "acccahhhaaabbb12736fffgh3647838bb1b2b3b2b");
        assert(n.to_string(32) == "mcnvkopachtalb36fughij6q3sr");
        assert(o.to_string(33) == "-mcnvkopachtalb36fughij6q3sr");
        assert(z.to_string(36) == "ghfsdkfjghsfkdjfgslfgdfk2r5g2rfefewfrt45t356y76ii7juytrerge1gjhdfg123");
    }{
        NN a("32768");
        a.assign("14385734543580723457", 16);
        assert(a == NN("14385734543580723457", 16));
        a.assign("14385734543580723457", 19);
        assert(a == NN("14385734543580723457", 19));
        a.assign("14385734543580723457", 23);
        assert(a != NN("14385734543580723457", 26));
        NN b;
        for (int i = 2; i <= 36; i++)
        {
            assert(b.to_string(i) == "0"); 
        }
    }
}

void test_mul()
{
    {
        NN a, b, c;
        mul(a, b, c);
        assert(c.is_zero());
    }
    {
        NN a, b(0), c;
        mul(a, b, c);
        assert(c.to_string() == "0");    
    }
    {
        NN a("76547234893209348945874575789508934903420"), b;
        NN c("12358965784784893"), d(98), z;
        mul(a, b, z);
        assert(z.is_zero());
        mul(b, a, z);
        assert(z.is_zero());
        mul(a, c, z);
        assert(eq(z, NN("946044656965066626408016115660230046048806161024430034060")));
        mul(a, d, z);
        assert(eq(z, NN("7501629019534516196695708427371875620535160")));
    }
    {
        NN a("123456789");
        NN b("123456789123456789");
        NN res("15241578765432099750190521");
        a.mul(b);
        assert(eq(a, res));
    }
    {
        NN a("123456789");
        NN res("15241578750190521");
        a.mul(a);
        assert(eq(a, res));
    }
    {
        NN a("123456789");
        NN b("123456789123456789");
        NN res("15241578765432099750190521");
        mul(a, b, a);
        assert(eq(a, res));
    }
    {
        NN a("123456789");
        NN b("123456789123456789");
        mul(a, b, b);
        assert(eq(b, NN("15241578765432099750190521")));
    }
    {
        NN a(4294967295LU);
        NN b;
        mul(a, a, b);
        assert(eq(b, NN("18446744065119617025")));
    }
    {
        NN a("FFFFFFFFFFFFFFFF", 16);
        NN b;
        mul(a, a, b);
        assert(eq(b, NN("340282366920938463426481119284349108225")));
    }
    {
        NN a("345234501298374"), b("-983757"), res;
        mul(a, b, res);
        assert(eq(res, NN("-339626857293784511118")));
        mul(b, a, res);
        assert(eq(res, NN("-339626857293784511118")));
        NN c("-345234501298374"), d("983757");
        mul(c, d, res);
        assert(eq(res, NN("-339626857293784511118")));
        mul(d, c, res);
        assert(eq(res, NN("-339626857293784511118")));
    }
    {
        NN a(123456), b(24680);
        a.bits_reserve(100);
        slen_t cap0 = a.cap;
        a.mul(b);
        slen_t cap1 = a.cap;
        assert(cap0 == cap1);
    }
}

void test_sqr()
{
    {
        NN a("-123456789"), res;
        sqr(a, res);
        assert(eq(res, NN("15241578750190521")));
    }{
        NN a("-123456789123456789123456789"), res;
        sqr(a, res);
        assert(eq(res, NN("15241578780673678546105778281054720515622620750190521")));
    }{
        NN a("FFFFFFFFFFFFFFFF", 16), res;
        sqr(a, res); assert(eq(res, NN("340282366920938463426481119284349108225")));
    }{
        NN a("261166390872565"), res;
        sqr(a, res);
        assert(eq(res, NN("68207883721401402942069679225")));
    }{
        NN x("21261716086314470342352740317850259437076309147311283139584986650769023112070970234843737506581730689302223122031565910399432695318786427954499147911397096162546854805063223049276111819611702626014056668082698428994626150852076691178381576499858171717931900774314358739310199411748428366705711401392812472323396539681847960385888113741516397088786748283182433659104473521400381560404282940768256368134747670384309751759255738258948674834442459351818464535236715300721624502344823709812421080889770010170238820883172633274646177691050550154150831875997442178287895442656208584529761244786165044114133735596751789080231435815193001508751900813452196739931959096679816010695226621781090364420857904323070880101198238340975356815988424218889476995111534031026113894819330057719028211998113808427496833457431647273894632880456524722815946904805561692975909840642219341777738559223213744936000794224679572850748985098871816534254051995019226617066789896847131379873409111591234994739726840327101120314893074709154099392819619377062855858191014253888196303644236792582236467691727616100879765670396656571665785454222956822145111231022314979699564732476437197682912725228512216308843822470447679673381636862615093973422195987881384872596058620930331959432994909239573513571765071290930951469726466492794405422060540919916385940542352080372053390611226435985907554953095845453645439253614283350540793721485769682816058588108668529420655529930230008792423");
        NN mr, sr;
        sqr(x, sr);
        mul(x, x, mr);
        assert(eq(sr, mr));
    }{
        NN a(1234);
        a.bits_reserve(100);
        slen_t cap0 = a.cap;
        a.sqr();
        slen_t cap1 = a.cap;
        assert(cap0 == cap1);
    }
}

void test_kmul()
{
    {
        NN a("123456789"), b("123456789"), c;
        kmul(a, b, c);
        assert(eq(c, NN("15241578750190521")));
    }
    {
        NN a("123456789645345454433565"), b("12345678912344577345"), c, d;
        kmul(a, b, c);
        mul(a, b, d);
        assert(eq(c, d));
    }
    {
        NN a, b, c, d;
        for (int i = 0; i < 15; i++)
        {
            create_big(a, 150);
            create_big(b, 150);
            kmul(a, b, c);
            mul(a, b, d);
            assert(eq(c, d));
            if (i & 1)
            {
                kmul(a, b, a);
                assert(eq(a, d));
            }
            else
            {
                b.kmul(a);
                assert(eq(b, d));
            }
        }
    }
}

void test_ksqr()
{
    {
        NN a("-123456789"), b, res;
        ksqr(a, res);
        assert(eq(res, NN("15241578750190521")));
        ksqr(b, res);
        assert(res.is_zero());
    }
    {
        NN a("16158503039417698314384037344878694324680742803587455484472260145626181232823105094609582067234949516440380598200430713907558648260168592343606325063405368414664799634798770137573188975647227632527500574967342497869185120677904929055092794689043588162497814115391580237240765753927314957610808555481953218839399468746177681961280635985777116657480297919651011778729062233777172196274269454168545630726130747048979353490487284178367280004761561058965043465792519393822714983002996704343992871425325657995590580009663274930698402305145290537250908528951674988824793651618050147449672523224061216263968385221040105652224");
        NN c, d;
        ksqr(a, c);
        sqr(a, d);
        assert(eq(c, d));
        assert(eq(c, NN("261097220474870994485893761582463012762496889583233066016945328308126523305914339956007596581876316207965769426245343681943187738821126451130542116625401122804336498594338150780579628515879193290778786946000002531427531164605107466695860207336270921803996747419851699925764137093708876460605444898678416226735036669183007128011151454339241114404779260103601491138342415854449886693195277799602869419371958598754304736541682354345273543160202191939559462918644876513258331523340507873274391595158730246027929540470110178195257568239291362170739342707797163574647831344070304095114816483889517686021235109674951368456048337904218292421160258322829374201238847854005581932809819680062647076737481370983191951941190155902734149558189911640505785640587894599046007103182533624678279694375708012103144162145661707622034672676013573987998429999878852730537507752594125847362629112874673436863079228204597634437572681862673177357368470127001191814193587789533814193852428550882449965211775253512320062582483717747091415378754740459781167955591849518435481636724538659100797842930620374513510505353640221643944753025110175584528541855068008148062856928661441743036851151397012739642092225003992965544911392918989556120610183226348312436146176")));
        a.set_neg();
        ksqr(a, d);
        assert(eq(c, d));
    }
    {
        NN a, c, d;
        for (int i = 0; i < 15; i++)
        {
            create_big(a, 150);
            ksqr(a, c);
            sqr(a, d);
            assert(eq(c, d));
            ksqr(a, a);
            assert(eq(a, d));
        }
    }
}

void test_cmp()
{
    {
        NN a, b("1"), c("-1");
        assert(cmp(a, a) == 0);
        assert(cmp(b, b) == 0);
        assert(cmp(c, c) == 0);
        assert(cmp(a, b) < 0);
        assert(cmp(b, a) > 0);
        assert(cmp(a, c) > 0);
        assert(cmp(c, a) < 0);
        assert(cmp(b, c) > 0);
        assert(cmp(c, b) < 0);
    }{
        assert(cmp(NN("ABCD1", 16), NN("ABCD2", 16)) < 0);
        assert(cmp(NN("ABCDEFEF1", 16), NN("ABCDEFEF2", 16)) < 0);
        assert(cmp(NN("ABCDEFEFFF1", 16), NN("ABCDEFEFFF2", 16)) < 0);
        assert(cmp(NN("ABCD2", 16), NN("ABCD1", 16)) > 0);
        assert(cmp(NN("ABCDEFEF2", 16), NN("ABCDEFEF1", 16)) > 0);
        assert(cmp(NN("ABCDEFEFFF2", 16), NN("ABCDEFEFFF1", 16)) > 0);

        NN x("ABCDEFFF", 16), y("ABCDEEEE1", 16), z("ABCDEEEEFFFFF", 16);
        assert(cmp(x, x) == 0);
        assert(cmp(y, y) == 0);
        assert(cmp(z, z) == 0);

        NN z0, z1;
        assert(cmp(z0, z0) == 0);
        assert(cmp(z0, z1) == 0);
    }{
        NN a, b, c("0"), d("123"), e("456"), f("-456");
        assert(eq(a, b));
        assert(eq(a, c));
        assert(!neq(a, b));
        assert(neq(d, e));
        assert(cmp(a, b) == 0);
        assert(cmp(c, b) == 0);
        assert(cmp(b, c) == 0);
        assert(cmp(c, d) < 0);
        assert(cmp(a, d) < 0);
        assert(cmp(d, e) < 0);
        assert(cmp(e, d) > 0);
        assert(neq(e, f));
        assert(cmp(e, f) > 0);
        assert(cmp(f, e) < 0);
        NN x("1231314453465546546"), y("1231314453465546546");
        assert(cmp(x, y) == 0);
        assert(cmp(y, x) == 0);
        assert(cmp(x, a) > 0);
        assert(cmp(a, x) < 0);
        assert(cmp(x, d) > 0);
        assert(cmp(d, x) < 0);
        assert(eq(x, y));
        assert(!neq(x, y));
        NN z("3244354673465546546");
        assert(cmp(x, z) < 0);
        assert(cmp(z, z) == 0);
        assert(eq(z, z));
        NN u("65538"), v("65539");
        assert(neq(u, v));
        assert(!eq(u, v));
        assert(cmp(u, v) < 0);
        assert(cmp(v, u) > 0);
    }{
        NN a("-123"), b("123"), c;
        assert(cmp(a, b) < 0);
        assert(cmp(b, a) > 0);
        assert(cmp(a, c) < 0);
        assert(cmp(c, a) > 0);
    }{
        NN a("-123"), b("-123456"), c("-123");
        assert(cmp(a, a) == 0);
        assert(eq(b, b));
        assert(!neq(c, c));
        assert(neq(a, b));
        assert(cmp(a, b) > 0);
        assert(neq(b, a));
        assert(cmp(b, a) < 0);
        assert(cmp(a, c) == 0);
        assert(eq(a, c));
    }{
        NN a("372324152635243142353626165645454541234565712234", 8), b("7D350D59D51989D7963AE9659614E5D7949C", 16);
        NN c("10907088453317382792613053832476836327036060", 10), d("11111010011010100001101010110011101010100011001100010011101011110010110001110101110100101100101100101100001010011100101110101111001010010011100", 2);
        assert(cmp(a, b) == 0);
        assert(cmp(a, c) == 0);
        assert(cmp(a, d) == 0);
        assert(cmp(b, c) == 0);
        assert(cmp(b, d) == 0);
        assert(cmp(c, d) == 0);
    }{
        assert(lt(NN("12345678901234567890"), NN("912345678901234567890")));
        assert(gt(NN("912345678901234567890"), NN("12345678901234567890")));
        assert(elt(NN("12345678901234567890"), NN("912345678901234567890")));
        assert(egt(NN("912345678901234567890"), NN("12345678901234567890")));
        assert(elt(NN("12345678901234567890"), NN("12345678901234567890")));
        assert(egt(NN("912345678901234567890"), NN("912345678901234567890")));
    }{
        NN a(131234);
        assert((short)3 < a); assert((int)3 < a); assert((long)3 < a); assert((long long)3 < a);
        assert((unsigned short)3 < a); assert((unsigned int)3 < a); assert((unsigned long)3 < a); assert((unsigned long long)3 < a);
        assert(a > (short)3); assert(a >= (int)3); assert(a >= (long)3); assert(a >= (long long)3);
        assert(a > (unsigned short)3); assert(a >= (unsigned int)3); assert(a >= (unsigned long)3); assert(a >= (unsigned long long)3);
        assert(a >= (bool)3); assert(a < 131235); assert(a <= 131235);
        assert(a == (int)131234); assert(a == (long)131234); assert(a == (long long)131234);
        assert(a == (unsigned int)131234); assert(a == (unsigned long)131234); assert(a == (unsigned long long)131234);      
        assert((int)131234 == a); assert((long)131234 == a); assert((long long)131234 == a);
        assert((unsigned int)131234 == a); assert((unsigned long)131234 == a); assert((unsigned long long)131234 == a);
        assert(a != (int)131235); assert(a != (long)131235); assert(a != (long long)131235);
        assert(a != (unsigned int)131235); assert(a != (unsigned long)131235); assert(a != (unsigned long long)131235);
    }{
        NN a("-12342342424234234"), b("12342342424234234"), c("12313");
        assert(cmp(a, b) < 0); assert(cmp(a, c) < 0);
        assert(cmp_abs(a, b) == 0); assert(cmp_abs(a, c) > 0); assert(cmp_abs(b, c) > 0);
    }
}

void test_add()
{
    {
        NN a, b, c, res;
        add(a, b, c);
        assert(eq(c, res));
        assert(res.is_zero());
    }
    {
        NN a(1234567), b, c, res(1234567);
        add(a, b, c);
        assert(eq(c, res));
    }
    {
        NN a, b(1234567), c;
        add(a, b, c);
        assert(eq(c, 1234567));
    }
    {
        NN a("567822223333322222445645351452345234535222222229");
        NN b("326345234534534534535245234523452345342345");
        NN c;
        add(a, b, c);
        assert(eq(c, NN("567822549678556756980179886697579757987567564574")));
        swap(a, b);
        add(a, b, c);
        assert(eq(c, NN("567822549678556756980179886697579757987567564574")));
        a.add(b);
        assert(eq(a, NN("567822549678556756980179886697579757987567564574")));
    }
    {
        NN a("326345234534534");
        NN b("567822223333322222445645351452345234535222222229");
        NN c;
        add(a, b, c);
        assert(eq(c, NN("567822223333322222445645351452345560880456756763")));
        swap(a, b);
        add(a, b, c);
        assert(eq(c, NN("567822223333322222445645351452345560880456756763")));
        a.add(b);
        assert(eq(a, NN("567822223333322222445645351452345560880456756763")));
    }
    {
        NN a;
        NN b("567822223333322222445645351452345234535222222229");
        NN c;
        add(a, b, c);
        assert(eq(c, NN("567822223333322222445645351452345234535222222229")));
        swap(a, b);
        add(a, b, c);
        assert(eq(c, NN("567822223333322222445645351452345234535222222229")));
        a.add(b);
        assert(eq(a, NN("567822223333322222445645351452345234535222222229")));
    }
    {
        NN a("123456789123456789");
        a.add(a);
        assert(eq(a, NN("246913578246913578")));
    }
    {
        NN a("FFFFFFFFFFFFFFFFFFFF", 16);
        NN b("499602D2", 16);
        NN res("1000000000000499602D1", 16);
        a.add(b);
        assert(eq(a, res));
    }
    {
        NN a("FFFFFFFFFFFFFFFFFFFF", 16);
        a.add(a);
        assert(eq(a, NN("2417851639229258349412350")));
    }
    {
        NN a("1208925819614629174706175"), b("1234567890"), c;
        c.bits_reserve(100);
        add(neg(a), b, c);
        assert(eq(c, NN("-1208925819614627940138285")));
        add(a, neg(b), c);
        assert(eq(c, NN("1208925819614627940138285")));
        add(neg(a), neg(b), c);
        assert(eq(c, NN("-1208925819614630409274065")));
        swap(a, b);
        add(a, b, c);
        assert(eq(c, NN("1208925819614630409274065")));
        add(neg(a), b, c);
        assert(eq(c, NN("1208925819614627940138285")));
        add(a, neg(b), c);
        assert(eq(c, NN("-1208925819614627940138285")));
        add(neg(a), neg(b), c);
        assert(eq(c, NN("-1208925819614630409274065")));
    }
    {
        NN a("-123456789"), b("123456789"), c("123456789");
        assert(add(b, a).is_zero());
        assert(add(a, b).is_zero());
        assert(add(a, c).is_zero());
        NN d("-123456789"), e("1234"), f("1234567890000"), g;
        assert(eq(add(d, e), NN("-123455555")));
        assert(eq(add(d, f), NN("1234444433211")));
        assert(eq(add(d, g), d));
        assert(eq(add(g, d), d));
    }
    {
        NN a("45634532785322123456"), b("674576854378");
        a.bits_reserve(100);
        b.bits_reserve(100);
        add(a, a, a); assert(eq(a, NN("91269065570644246912")));
        add(a, b, a); assert(eq(a, NN("91269066245221101290")));
        add(a, b, b); assert(eq(b, NN("91269066919797955668")));
        a.assign("-45634532785322123456");
        b.assign("674576854378");
        add(a, a, a); assert(eq(a, NN("-91269065570644246912")));
        add(a, b, a); assert(eq(a, NN("-91269064896067392534")));
        add(a, b, b); assert(eq(b, NN("-91269064221490538156")));
    }
}

void test_sub()
{
    {
        NN a, b, c, res; sub(a, b, c);
        assert(eq(c, res)); assert(res.is_zero());
    }{
        NN a(1234567), b, c, res(1234567);
        sub(a, b, c); assert(eq(c, res));
    }{
        NN a("123412345123456767123456751234567123456767"), b, c;
        sub(a, b, c); assert(eq(c, a));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("326345234534534534535245234523452345342345"), c;
        sub(a, b, c); assert(eq(c, NN("567821896988087687911110816207110711082876879884")));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("326345234534534534535245234523452345342345");
        sub(a, b, a); assert(eq(a, NN("567821896988087687911110816207110711082876879884")));
    }{
        NN a("-567822223333322222445645351452345234535222222229"), b("-326345234534534534535245234523452345342345"), c;
        sub(a, b, c);
        assert(eq(c, NN("-567821896988087687911110816207110711082876879884")));
        swap(a, b);
        sub(a, b, c);
        assert(eq(c, NN("567821896988087687911110816207110711082876879884")));
    }{
        NN a("123456789123456789"), b("123456789123456789"), c;
        sub(a, b, c); assert(eq(c, 0));
        sub(a, a, c); assert(eq(c, 0));
        sub(b, b, c); assert(eq(c, 0));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("567822223333322222445645351452345234535222222229"), c(123456789);
        sub(a, b, c); assert(cmp(c, 0) == 0);
    }{
        NN a("567822223333322222445645351452345234535222222229");
        a.sub(a); assert(eq(a, 0));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("326345234534534534535245234523452345342345");
        a.sub(b); assert(!cmp(a, NN("567821896988087687911110816207110711082876879884")));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("326345234534534534535245234523452345342345");
        a.sub(b); assert(eq(a, NN("567821896988087687911110816207110711082876879884")));
    }{
        NN a("123456"), b("-1");
        assert(eq(sub(a, b), 123457));
        assert(eq(sub(b, a), -123457));
        assert(eq(sub(a, a), 0));
        assert(eq(sub(b, b), 0));

        NN c("-123456"), d("-1234567");
        assert(eq(sub(c, d), 1111111));
        assert(eq(sub(d, c), -1111111));

        NN e, f("-1234567");
        assert(eq(sub(e, f), 1234567));
        assert(eq(sub(f, e), -1234567));
        assert(eq(sub(f, f), 0));
        assert(eq(sub(e, e), 0));

        NN g("-12345678901234567890");
        NN h("-12345678901234567890");
        assert(eq(sub(g, h), 0));
    }{
        NN a("123456"), b("-1"), c;
        sub(a, b, c);
        NN d = c;
        add(d, c, d);
        assert(eq(c, 123457));
        assert(eq(d, 246914));
    }{
        NN a("123456"), b("2342947290847209");
        assert(eq(sub(a, b), NN("-2342947290723753")));
        NN c("0"), d("2342947290847209"), e("-2342947290847209");
        assert(eq(sub(c, d), NN("-2342947290847209")));
        assert(eq(sub(c, e), d));
    }{
        NN a("123456");
        a -= 456;
        assert(a == 123000);
    }
}

void test_fun(number_t a)
{
    a.bit_set(5);
}

void test_div()
{
    {
        NN a("0"), b("123456789123456789"), z(0), q, r;
        div(a, b, q, r);
        assert(eq(q, 0));
        assert(eq(r, 0));
        assert(div(b, z, q, r) == 0);

        div(a, b, q);
        assert(eq(q, 0));
        assert(eq(r, 0));
    }{
        NN a("123456789"), b("123456789123456789"), q, r;
        div(a, b, q, r);
        assert(eq(q, 0));
        assert(eq(r, a));
        div(a, b, q);
        assert(eq(q, 0));
        div(b, b, q);
        assert(eq(q, 1));
    }{
        NN a("123456789"), b("987654321"), q, r;
        div(a, b, q, r);
        assert(eq(q, 0));
        assert(eq(r, a));
        div(a, b, q);
        assert(eq(q, 0));
    }{
        NN a("223456789"), b("123456789"), q, r;
        div(a, b, q, r);
        assert(eq(q, 1));
        div(a, b, q);
        assert(eq(q, 1));
    }{
        NN a("123456789"), b("123456789"), q, r;
        div(a, b, q, r);
        assert(eq(q, 1));
        assert(eq(r, 0));
        div(a, b, q);
        assert(eq(q, 1));
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("326345234534534534535245234523452345342345"), q, r;
        div(a, b, q, r);
        assert(eq(q, NN("1739943")));
        assert(eq(r, NN("116921600600822787152359905990423226435894")));
        div(a, b, q);
        assert(eq(q, NN("1739943")));
        b.assign(a);
        div(a, b, q, r);
        assert(q.is_one());
        assert(r.is_zero());
        div(a, a, q, r);
        assert(q.is_one());
        assert(r.is_zero());
    }{
        NN a("567822223333322222445645351452345234535222222229"), b("32"), q, r;
        div(a, b, q, r);
        assert(eq(q, NN("17744444479166319451426417232885788579225694444")));
        assert(eq(r, 21));
        div(a, b, q);
        assert(eq(q, NN("17744444479166319451426417232885788579225694444")));
    }{
        NN a("ABCDABCDABCDABCDABCDABCD12345678901234567889", 16), b("CBCDABCDABCDABCDABCDABCD12345678901234511111", 16), q, r;
        div(a, b, q, r);
        assert(eq(q, 0));
        assert(eq(r, a));
    }{
        NN a("657393756583930575659302"), b("436793763783930575659302"), q, r;
        div(a, b, q, r);
        assert(eq(q, 1));
        assert(eq(r, NN("220599992800000000000000")));
        div(a, b, q);
        assert(eq(q, 1));
    }{
        NN a("64"), b("32"), q, r;
        div(a, b, q, r);
        assert(eq(q, 2));
        assert(eq(r, 0));
    }{
        NN a("105627020819278699202226221738072060416208264147469285199857699"), b("1358731003278066155"), q, r;
        NN qres("77739464665517744994685560511295222996256726"), rres("855556420008149169");
        div(a, b, q, r);
        assert(eq(q, qres));
        assert(eq(r, rres));
        div(a, b, q);
        assert(eq(q, qres));
    }{
        NN a("18442803419740635136"), b("4294115314"), q, r;
        NN qres("4294901759"), rres("4293197810");
        div(a, b, q, r);
        assert(eq(q, qres));
        assert(eq(r, rres));
        div(a, b, q);
        assert(eq(q, qres));
    }{
        NN a("47271288836269063960768557193460314004624922962982927"), b("202203122482853718328706"), q, r;
        NN qres("233781201080500338385962547211"), rres("36642103546837421443961");
        div(a, b, q, r);
        assert(eq(q, qres));
        assert(eq(r, rres));
    }{
        NN a("13898656758691282158262973353334429789850480821076586876041609464221096097620101985817142170844495889147765454349763664371000642307252973778");
        NN b("240156018875860169859445497983972818044767958495628437705");
        NN q, r;
        NN qres("57873447535269487061823839108720898352953114902535413457056778527438554574085619711");
        NN rres("233940950350928131075601779252813875807543128037569370523");
        div(a, b, q, r);
        assert(eq(q, qres));
        assert(eq(r, rres));
        div(a, b, q);
        assert(eq(q, qres));
    }{
        NN q, r;
        NN a("764189374923874453637484745392874");
        NN b("938374374383923938373");
        div(a, b, q, r);
        assert(q * b + r == a);
        div(b, a, q, r);
        assert(q * a + r == b);
        NN c("-764189374923874453637484745392874");
        NN d("938374374383923938373");
        div(c, d, q, r);
        assert(q * d + r == c);
        div(d, c, q, r);
        assert(q * c + r == d);
        NN e("34374923874453637484745392874");
        NN f("-656809124763121938374374383923938373");
        div(e, f, q, r);
        assert(q * f + r == e);
        div(f, e, q, r);
        assert(q * e + r == f);
        NN g("-89001111134374923874453637484745392874");
        NN h("-656809124763121938374374383923938373");
        div(g, h, q, r);
        assert(q * h + r == g);
        div(h, g, q, r);
        assert(q * g + r == h);
    }{
        NN a("41234523452356543634565436"), b(-123), c;
        div(a, b, a); assert(a == NN("-335240028067939379142808"));
        div(c, b, a); assert(a == 0);
    }{
        NN a("37401e908", 16), b(0x12ffef), c;
        div(a, b, c); assert(c == 11910);
    }{
        NN a("1234123412412341234124"), b("231412341234123412341234123412412");
        div(a, b, a, b); assert(a == 0); assert(b = NN("1234123412412341234124"));
        a.assign("1234123412412341234124");
        b.assign("231412341234123412341234123412412");
        div(a, b, b, a); assert(b == 0); assert(a = NN("1234123412412341234124"));
    }
    for (int i = 0; i < 2000; i++)
    {
        NN a, b, q, r;
        create_big(a, rand() % 100);
        create_big(b, rand() % 100);
        if (chance(10)) a.neg();
        if (chance(10)) b.neg();
        if (div(a, b, q, r))
        {
            assert(b.mul(q).add(r) == a);
        }
    }
}

void test_mod()
{
    {
        number_t a, b("8f439616ca2c520b577ff1bea", 18), bb(b), c;
        mod(a, b, c); assert(c == 0);
        mod(a, b, b); assert(b == 0);
        mod(a, bb, a); assert(a == 0);
    }{
        number_t a("-3a6a47b1161c1b8800590990b4c04c65b89642070", 13);
        number_t b("8f439616ca2c520b577ff1bea", 18), bb(b), c;
        mod(a, b, c); assert(c == NN("-6822147001369128807443405032605"));
        mod(a, b, b); assert(b == NN("-6822147001369128807443405032605"));
        mod(a, bb, a); assert(a == NN("-6822147001369128807443405032605"));
    }{
        number_t a("277953339371689316832154863864204694716"), b("8f439616ca2c520b577ff1bea", 18), bb(b), c;
        mod(a, b, c); assert(c == 0);
        mod(a, b, b); assert(b == 0);
        mod(a, bb, a); assert(a == 0);
    }
}

void test_and()
{
    {
        NN a("1231231234235443545645645345353534242"), b("67673453453453453453435645675653534242"), c;
        bit_and(a, b, c); assert(eq(c, NN("1210459275980800584861333892747133474")));
        bit_and(a, b, a); assert(eq(a, NN("1210459275980800584861333892747133474")));
        bit_and(a, b, b); assert(eq(b, NN("1210459275980800584861333892747133474")));
    }{
        NN a("986340586238271800287389303827383930"), b("985654180028738393067");
        a.bits_reserve(200);
        unit_t* dat0 = a.dat;
        bit_and(a, b, a);
        assert(eq(a, NN("888615112260060201578")));
        a.shl(1);
        bit_and(a, b, b);
        assert(eq(b, NN("595484520079951921344")));
        unit_t* dat1 = a.dat;
        assert(dat0 == dat1);
    }{
        NN a, b, c; bit_and(a, b, c); assert(eq(c, 0));
    }{
        NN a, b(123), c; bit_and(a, b, c); assert(eq(c, 0));
    }{
        NN a(3276888), b(3276888), c; bit_and(a, b, c); assert(eq(c, 3276888));
    }{
        NN a(-1), b(-1234124), c; bit_and(a, b, c); assert(c == 0);
    }{
        NN a(0xabcdef11); a.bit_and_word(~0xabcdef11); assert(a == 0);
        NN b(0xef11); b.bit_and_word(0xabcdef11); assert(b == 0xef11);
    }
}

void test_or()
{
    {
        NN a(int(0)), b(1), c(0x1234), d(0x5678), e(0x1111abcd);
        NN f(0x2233dcba), g(0x11111abcdULL), h(0x82233dcbaULL);
        a.bit_or(a); assert(a == 0);
        a.bit_or(b); assert(a == 1);
        c.bit_or(d); assert(c == 0x567c);
        e.bit_or(f); assert(e == 859045887);
        g.bit_or(h); assert(g == 0x93333ffffULL);
    }{
        NN a("1231231234235443545645645345353534242"), b("67673453453453453453435645675653534242"), c;
        bit_or(a, b, c);
        assert(eq(c, NN("67694225411708096414219957128259935010")));
        swap(a, b);
        bit_or(a, b, c);
        assert(eq(c, NN("67694225411708096414219957128259935010")));
    }{
        NN a("986340586238271800287389303827383930"), b("985654180028738393067");
        a.bits_reserve(200);
        unit_t* dat0 = a.dat;
        bit_or(a, b, a);
        assert(eq(a, NN("986340586238271897326457072505575419")));
        a.shl(1);
        bit_or(a, b, b);
        assert(eq(b, NN("1972681172476544184489003688856125439")));
        unit_t* dat1 = a.dat;
        assert(dat0 == dat1);
    }{
        NN a(-1), b(-1234124), c;
        bit_or(a, b, c); assert(c == 1234125);
    }{
        NN a; a.bit_or_word(0xabcdef11); assert(a == 0xabcdef11);
        NN b(0xef11); b.bit_or_word(0xabcd0000); assert(b == 0xabcdef11);
    }
}

void test_xor()
{
    {
        NN a("1231231234235443545645645345353534242"), b("67673453453453453453435645675653534242"), c;
        bit_xor(a, b, c);
        assert(eq(c, NN("66483766135727295829358623235512801536")));
        b = a;
        bit_xor(a, b, c);
        assert(c == 0);
        bit_xor(a, b, a);
        assert(a == 0);
        bit_xor(a, b, b);
        assert(eq(b, NN("1231231234235443545645645345353534242")));
    }{
        NN a("1231231234235443545645645345353534242"), b, c;
        bit_xor(a, b, c);
        assert(eq(c, NN("1231231234235443545645645345353534242")));
        bit_xor(a, b, a);
        assert(eq(a, NN("1231231234235443545645645345353534242")));
        bit_xor(a, b, b);
        assert(eq(b, NN("1231231234235443545645645345353534242")));        
    }{
        NN a("0"), b, c; bit_xor(a, b, c); assert(eq(c, 0));
    }{
        NN a, b(1), c; bit_xor(a, b, c); assert(eq(c, 1));
    }{
        NN a(-1), b(-1234124), c; bit_xor(a, b, c); assert(c == 1234125);
    }{
        NN a(0xabcdef11); a.bit_xor_word(0xabcdef11); assert(a == 0);
        NN b(0xabcd0000); b.bit_xor_word(0xabcdef11); assert(b == 0xef11);
    }
}

void test_shl()
{
    {
        NN a("123123123123"), c;
        a.shl(0); assert(eq(a, NN("123123123123")));
        a.assign("123123123123");
        a.shl(1); assert(eq(a, NN("246246246246")));
        a.assign("123123123123");
        a.shl(48); assert(eq(a, NN("34656078213589656234098688")));
        a.assign("123123123123");
        shl(a, 100, c); assert(eq(c, NN("156077100928845140500767189234098003509248")));
        a.assign("123123123123");
        shl(a, 63, c); assert(eq(c, NN("1135610370902905855478945808384")));
        a.assign("123123123123");
        a.shl(100); assert(eq(a, NN("156077100928845140500767189234098003509248")));
    }{
        NN a(1234); a.bits_reserve(1000);
        a.shl(1); assert(eq(a, NN("2468")));
        a.shl(2); assert(eq(a, NN("9872")));
        a.shl(3); assert(eq(a, NN("78976")));
        a.shl(50); assert(eq(a, NN("88919071042803073024")));
    }{
        NN a(1234), b(a);
        for (size_t i = 0; i < 100; i++)
        {
            a.shl(1); assert(a == b.mul(2));
        }
    }
}

void test_shr()
{
    {
        NN a("123132423423423423423123123"), c;
        shr(a, 16, c); assert(eq(c, NN("1878851675772452139635")));
        a.assign("8904523397800877688990732422458245342342342342317678584756575843901902834754723123");
        shr(a, 13, a); assert(eq(a, NN("1086977953833114952269376516413360027141399211708700999115792949695056498383144")));
        shr(a, 39, a); assert(eq(a, NN("1977201379910393283843180172722641195180402449675422756837711767175")));
        shr(a, 129, a); assert(eq(a, NN("2905236315653373508593086217")));
        shr(a, 6000, a); assert(eq(a, 0));
        shr(a, 1, a); assert(eq(a, 0));
        shr(a, 1000, a); assert(eq(a, 0));
        a.assign(7);
        shr(a, 3, c); assert(eq(c, 0));
        a.assign(15);
        shr(a, 3, c); assert(eq(c, 1));
        a.assign("123132423423423423423123123");
        a.shr(0); assert(eq(a, NN("123132423423423423423123123")));
        a.assign("123132423423423423423123123");
        a.shr(18); assert(eq(a, NN("469712918943113034908")));
    }{
        NN a("123132423423423423423123123"), c;
        a.shr(16); assert(a == NN("1878851675772452139635"));
        a.shr(32); assert(a == NN("437454245000"));
        a.shr(33); assert(a == 50);
    }{
        NN a("ef1daf2cc3542565b63a546435634095f3490b0f87872357398b0c045723970500", 16), b(a);
        for (size_t i = 0; i < 100; i++)
        {
            a.shr(1); assert(a == b.div(2));
        }
    }
}

void test_not()
{
    {
        NN a, b(1), c(2), d(-3);
        assert(~a == 0);
        assert(~b == 0);
        assert(~c == 1);
        assert(~d == 0);
    }
    {
        NN a("10010010101111101111100000000110", 2);  // 32bits
        NN b("01101101010000010000011111111001", 2);
        bit_not(a, a);
        assert(a == b);
        NN c("-110010010101111101111100000000110", 2);  // 33bits
        NN d("-001101101010000010000011111111001", 2);
        c.bit_not();
        assert(c == d);
        NN e("1111110010010101111101111100000000110", 2);  // 37bits
        NN f("0000001101101010000010000011111111001", 2);
        assert(~e == f);
    }
    {
        NN a("10010010101111101111100000000110", 2);
        NN b = a;
        unit_t* dat0 = a.dat;
        a.bit_not();
        a.bit_and(b);
        unit_t* dat1 = a.dat;
        assert(a == 0);
        assert(dat0 == dat1);
    }
}

void test_pow()
{
    {
        NN a("123456789"), c;
        pow(a, 12, c);
        assert(eq(c, NN("12536598752890180325268918101607183995023033525376668780118925489446327290480100125725042366959121")));
        a.set_one();
        pow(a, 123456789, c);
        assert(eq(c, 1));
        a.set_zero();
        pow(a, 123456789, c);
        assert(eq(c, 0));
        pow(a, 0, c);
        assert(eq(c, 1));
    }{
        NN a(33); pow(a, a, a); assert(eq(a, NN("129110040087761027839616029934664535539337183380513")));
    }{
        NN a, b("12"), c; pow(a, b, c); assert(eq(c, 0));
    }{
        NN a, b; pow(a, 0, b); assert(eq(b, 1));
    }{
        NN a(123), b; pow(a, 0, b); assert(eq(b, 1));
    }{
        NN a(123); a.pow(0); assert(eq(a, 1));
        NN b; b.pow(123); assert(eq(b, 0));
    }{
        NN a(12);
        a.pow(12); assert(eq(a, NN("8916100448256")));
        pow(a, 3, a); assert(eq(a, NN("708801874985091845381344307009569161216")));
    }{
        NN a(-1); a.pow(3); assert(a == -1);
        a.pow(4); assert(a == 1);
        a.assign(-12); a.pow(12); assert(a == 8916100448256);
        a.assign(-12); a.pow(13); assert(a == -106993205379072LL);
    }{
        NN a(2); a.pow(16); assert(a == 65536);
        a.assign(2); a.pow(32); assert(a == 4294967296);
        a.assign(2); a.pow(71); assert(a == NN("2361183241434822606848"));
    }
}

void test_pom()
{
    {
        NN a("123456789"), b("12"), c("12345"), res;
        pom(a, b, c, res);
        assert(eq(res, 4926));
    }{
        NN a(65535), b(65539), c("10000000000000000000"), res;
        pom(a, b, c, res);
        assert(eq(res, NN("6493740081787109375")));
    }{
        NN a(1), b(0), c(1), res;
        pom(a, b, c, res);
        assert(eq(res, 0));
    }{
        NN a(123), b(1000), c(126), res;
        a.pom(b, c);
        assert(eq(a, 81));
    }{
        NN a(0), b(0), c(1), res;
        pom(a, b, c, res);
        assert(eq(res, 0));
    }{
        NN a(0), b(1), c(1), res;
        pom(a, b, c, res);
        assert(eq(res, 0));
    }{
        NN a("-12341234123424"), b("18113"), c("2307482378456903845"), res;
        pom(a, b, c, res); assert(eq(res, NN("-442381454734388839")));
        pom(a, 119876, c, res); assert(eq(res, NN("1111416039757584216")));
    }{
        NN a("12341234123424"), b(234567), c(2342342342);
        pom(a, b, c, c); assert(!neq(c, NN("2168243588")));
    }{
        NN a("11"), b(1234567), c(2342342342);
        pom(a, b, c, c); assert(!neq(c, NN("1639424989")));
    }{
        NN a("2389542389475234adfs8572354723450872asdfasdfasdfasd34572340928745093875239485723945823427509457823487523954723", 36);
        NN b("rrrrrrrafgrgfsdgdfkljasdfhiuhsdffhaskdjfhasfjsafhafasfas", 36);
        NN c("bnadlskjahsdfaruihkjddddddddddddddddddhfaslkdjfhkfjasdasdfkljsdfhaslkdfhaskdlfh", 36);
        pom(a, b, c, c);
        assert(c.bits_count() == 407);
    }{
        NN res;
        pom(NN("23894572389752374856"), NN("90856895467843783478234783478"), NN("585765756848934"), res);
        assert(res == NN("49110959974582"));
        pom(NN("75674839329487547623728181092309349485111233127567086345637"), NN("454356378783478234783478"), NN("5857657568489"), res);
        assert(res == NN("2172436569126"));
        pom(NN("6467389208756231480"), NN("12346465974213684735627389"), NN("595746385857657568489"), res);
        assert(res == NN("395420613459476482273"));
        pom(NN("756748393294875476237"), NN("981432897860900349485485900001232345545689675483490454356378783478234783478"), NN("83467900057657568489645327"), res);
        assert(res == NN("53803471142475144287645784"));
    }
}

void test_property()
{
    {
        NN a, b(12347), c(12348), d(65536);
        assert(!a.is_odd()); assert(a.is_even()); assert(a.is_zero()); assert(!a.is_po2());
        assert(b.is_odd()); assert(!b.is_even());
        assert(c.is_even()); assert(!c.is_odd());
        assert(d.is_even()); assert(d.is_po2());
        NN e(1); assert(!e.is_zero()); assert(e.is_one());
        e <<= 300; assert(e.is_po2());
        assert(!NN(-65536).is_po2());
        assert(!NN(-23452346).is_po2());
    }{
        NN a(1), b(1), c(3), d(0xFFFF);
        assert(a.is_po2());
        b.shl(1027); assert(b.is_po2());
        assert(!c.is_po2()); assert(!d.is_po2());
        NN e("107839786668602559178668060348078522694548577690162289924414440996864"); assert(e.is_po2());
        NN z; assert(!z.is_po2());
    }{
        NN a(1), b(-1), c(0);
        assert(a.is_pos()); assert(!a.is_neg()); assert(!b.is_pos());
        assert(b.is_neg()); assert(!c.is_pos()); assert(!c.is_neg());
    }
}

void test_bits()
{
    {
        NN a, b;
        const NN& const_a_ref = a;
        a.bit_set_one(0); assert(eq(a, 1)); assert(const_a_ref[0] == 1);
        a.bit_set_one(1); assert(eq(a, 3)); assert(const_a_ref[1] == 1);
        a.bit_set_one(2); assert(eq(a, 7)); assert(const_a_ref[2] == 1);
        a.bit_set_one(3); assert(eq(a, 15)); assert(const_a_ref[3] == 1);
        a.bit_set_one(4); assert(eq(a, 31)); assert(const_a_ref[4] == 1);
        a.bit_set_one(16); assert(eq(a, 65567)); assert(const_a_ref[16] == 1);

        assert(a[0] == 1); assert(1 == a[0]);
        assert(!a[0] == 0); assert(~a[0] == 0);
        assert(a[0] == true); assert(true == a[0]); assert(a[0] != false); assert(false != a[0]);

        a[32] = 1; assert(a(2) == "100000000000000010000000000011111"); assert(const_a_ref[32] == 1);
        a[33] = 1; assert(a(2) == "1100000000000000010000000000011111"); assert(const_a_ref[33] == 1);
        a[34] = 1; assert(a(2) == "11100000000000000010000000000011111"); assert(const_a_ref[34] == 1);
        a[3] = a[1]; assert(a(2) == "11100000000000000010000000000011111");
        a[3] = a[5]; assert(a(2) == "11100000000000000010000000000010111");
        a[0] = 0; assert(a(2) == "11100000000000000010000000000010110"); assert(const_a_ref[0] == 0);
        a[2] = 0; assert(a(2) == "11100000000000000010000000000010010"); assert(const_a_ref[2] == 0);
        a[34] = 0; assert(a(2) == "1100000000000000010000000000010010"); assert(const_a_ref[34] == 0);
        a.bit_set_zero(128); assert(a(2) == "1100000000000000010000000000010010");
        b.bit_set_zero(0); assert(b(2) == "0");
        b[128] = 0; assert(b(2) == "0");

        const number_t& r = a;  // just test compiling
        int x = a[9] != 1;
        x = r[9] == 1;
        //++a[8]++;
        //--a[8]--;
        x = a[1] + a[2] - a[3] * a[4];
        assert(x == 1);
    }{
        NN a(0x123FFabcdefULL); //100100011 1111111110101011 1100110111101111
        const NN& ref = a;
        assert(a.bits_count() == 41);
        assert(a[0]); assert(a.bit_at(0)); assert(a[1]); assert(a.bit_at(1)); assert(ref[2]); assert(ref.bit_at(2));
        assert(a[3]); assert(a.bit_at(3)); assert(!a[4]); assert(!a.bit_at(4));
        assert(a[5]); assert(a.bit_at(5)); assert(!a.bit_at(9)); assert(a.bit_at(17)); assert(!a.bit_at(18));
        assert(a.bit_at(31)); assert(ref.bit_at(32)); assert(a.bit_at(33)); assert(!ref[34]); assert(!a.bit_at(35));
        assert(!a.bit_at(36)); assert(a.bit_at(37)); assert(!a.bit_at(38)); assert(!a.bit_at(39)); assert(ref[40]);

        NN b, c(1), d(2), e(0xf), f(0x1f); 
        assert(b.bits_count() == 0);
        assert(c.bits_count() == 1);
        assert(d.bits_count() == 2);
        assert(e.bits_count() == 4);
        assert(f.bits_count() == 5);

        NN g(0xaff2), h(0x3fff), i(0x7defcdef), j("9132ddd2347defcdeffffffaaaaaa", 16);
        assert(g.bits_count() == 16);
        assert(h.bits_count() == 14);
        assert(i.bits_count() == 31);
        assert(j.bits_count() == 116);
    }{
        NN a(-0x123FFabcdefLL); //100100011 1111111110101011 1100110111101111
        const NN& ref = a;
        assert(a.bits_count() == 41);
        assert(a[0]); assert(a.bit_at(0)); assert(a[1]); assert(a.bit_at(1));
        assert(ref[2]); assert(ref.bit_at(2)); assert(a[3]); assert(a.bit_at(3)); assert(!a[4]); assert(!a.bit_at(4));
        assert(a[5]); assert(a.bit_at(5)); assert(!a.bit_at(9)); assert(a.bit_at(17)); assert(!a.bit_at(18));
        assert(a.bit_at(31)); assert(ref.bit_at(32)); assert(a.bit_at(33)); assert(!ref[34]); assert(!a.bit_at(35));
        assert(!a.bit_at(36)); assert(a.bit_at(37)); assert(!a.bit_at(38)); assert(!a.bit_at(39)); assert(ref[40]);
    }{
        NN a("111", 2);
        for (int i = 0; i < 43; i++)
        {
            assert(a.tzbits_count() == i);
            assert(a.bits_count() == i + 3);
            a.shl(1); 
        }
        NN b; assert(b.tzbits_count() == 0);
    }{
        NN a;
        a[1] = a[3] = a[5] = a[7] = a[9] = 1; a[9] = 0; assert(a == 170);
        a[1] = a[3] = 0; assert(a == 160);

        a.assign("-ffffffff", 16);
        a.bit_set(0); assert(a(16) == "-ffffffff");
        a.bit_set(0, 0); assert(a(16) == "-fffffffe");
        a.bit_set(1); assert(a(16) == "-fffffffe");
        a.bit_set(32); assert(a(16) == "-1fffffffe");
        a.bit_set(33); assert(a(16) == "-3fffffffe");
        a.bit_set(33, 0); assert(a(16) == "-1fffffffe");
        a.bit_set(1000, 0); assert(a(16) == "-1fffffffe");
        a.bit_set(63, 1); assert(a(16) == "-80000001fffffffe");
        a.bit_set(64, 1); assert(a(16) == "-180000001fffffffe");
        a.bit_set(65, 1); assert(a(16) == "-380000001fffffffe");
        a.bit_set(0, 1); assert(a(16) == "-380000001ffffffff");
    }
}

void test_swap()
{
    NN a, b, c("123123423325345345"), d("989787673463478378293892389238923873478434378");
    swap(a, b);
    swap(c, d);
    assert(!a);
    assert(a == b);
    assert(c == NN("989787673463478378293892389238923873478434378"));
    assert(d == NN("123123423325345345"));
    swap(a, d);
    assert(a == NN("123123423325345345"));
    assert(!d);
}

void test_operators()
{
    {
        NN a, b(1), c("ffffffff", 16), d("ffffffffffffffff", 16);
        --a; assert(a == -1);
        ++a; assert(a == 0);
        a++; assert(a == 1);
        a--; assert(a == 0);
        --a; assert(a == -1);
        --a; assert(a == -2);
        b++; assert(b == 2);
        --b; assert(b == 1);
        c++; assert(c == 0x100000000ULL);
        c--; assert(c == 0xffffffff);
        ++d; assert(d == NN("10000000000000000", 16));
        --d; assert(d == NN("ffffffffffffffff", 16));
        --d; assert(d == NN("fffffffffffffffe", 16));
        NN e(5);
        --e; --e; --e; e--; e--; assert(e == 0);
        --e; --e; --e; e--; e--; assert(e == -5);
        ++e; ++e; assert (e == -3); ++e; e++; e++; assert(e == 0);
        ++e; ++e; assert (e == 2); ++e; e++; e++; assert(e == 5);
    }
    {
        NN a = (int)123, b = (short)-7, c = (char)-120, d = (long)780000, e = (long long)3;
        assert(a / b * c - d + e == -777957);
        assert(a + b - c * d / e == 31200116);
        assert(a * b / c * d / e == 1820000);
        assert(a - b - c - d - e == -779753);
        assert(a / b / c * d * e == 0);
    }
    {
        NN a = 1345, b = 89984, c = 9999999, d = 'c';
        NN res = (a * ((a + b) * c) - d) * b * c * d;
        assert(res == NN("109428737608448376115036875264"));
        assert(res != NN("109428737608448376115036875265"));
    }
    {
        NN a = 785345, b = 0x9989984cabcdeULL, c = short(8888), d = true;
        NN res = (a * b / c) * (b - d);
        assert(res == NN("644651315202522789465976218741175"));
    }
    {
        NN a("abcdccccdddddeeeeef123456", 16), b = a % 1000, c = short(8888);
        NN res = a * b * b % c; 
        assert(res == 2984);
    }
    {
        NN a("abcdccccdddddeeeeef123456", 16), b = 0xff0000, c = short(8888), d("222222222222222222222222111111111111111111111111");
        NN res = ((((a | b) ^ c) << 100) >> 9) & d; 
        assert(res == NN("222222222210923702491943018864797904674232467456"));
        ++a++;
        b--;
        b -= 1234;
        res = (a + b - c % d) ^ ((a & b) | c);
        assert(res == NN("850731737829703704754446876413"));
    }
    {
        NN a = 12345678;
        NN b = -a;
        NN c = +b;
        assert(a != b);
        assert(a + b == 0);
        assert(c == b);
    }
    {
        NN a(123), b(456);
        assert(a < b); assert(a <= b); assert(a <= a); assert(a + b > a); assert(a + b > b);
        assert(a + b >= a); assert(b >= b); assert(a && b); assert(a || b); assert(!(a && 0)); assert(a || 0);
        assert(!(0 && b)); assert(0 || b);
    }
    {
        NN a, b("14305ff8724375aaabb", 16), c(b), d(b), e(b), f(b), g(b), h(b), i(b), j(b), k(b);
        a %= 1234; assert(a == 0);
        a++; assert(a == 1); assert(a);
        ++a; assert(a == 2); assert(a);
        --a; assert(a == 1);
        a--; assert(!a);

        b %= 1234; assert(b == 1189);
        c *= 1234; assert(c == NN("7353073496833023760071014"));
        d /= 1234; assert(d == 4828792989049475923ULL);
        e &= 1234; assert(e == 146);
        f |= 1234; assert(f == NN("5958730548487053291259"));
        g ^= 1234; assert(g == NN("5958730548487053291113"));
        h += 1234; assert(h == NN("5958730548487053291405"));
        i -= 1234; assert(i == NN("5958730548487053288937"));
        j <<= 9; assert(j == NN("3050870040825371284567552"));
        k >>= 3; assert(k == NN("744841318560881661271"));
    }
    {
        NN a = 123, b = a++; assert(b == 123 && a == 124);
        NN c = ++a; assert(c == 125 && a == 125);
        a = ++a; assert(a == 126);
        a = a++; assert(a == 127);
    }
}

void test_string_assignment()
{
    {
        string_t a, b;
        a.assign(b);
        assert(a == b);
        assert(a == "");
    }{
        string_t a("asdfasdfasfsa"), b;
        a.assign(b);
        assert(a == b);
        assert(a == "");
    }{
        string_t a, b("asdfasdfasfsa");
        a.assign(b);
        assert(a == b);
        assert(a == "asdfasdfasfsa");
    }{
        string_t a("abcde"), b("1234567890");
        a.assign(b); assert(a == b); assert(a == "1234567890");
        string_t c("1234567890"), d("abcde");
        c.assign(d); assert(c == d); assert(c == "abcde");
        string_t e("1111"), f("1111");
        e.assign(f); assert(e == f); assert(e == "1111");
    }
    {
        string_t a("xxxxxxxxxxxxxxxxxxxx"), b;
        a.assign("123"); assert(a == "123");
        a.assign("4567890", 3); assert(a == "456");
        a.assign("4567890", 0); assert(a == "");
        b.assign("123"); assert(b == "123");
        b.assign("4567890"); assert(b == "4567890");
        a.assign("xxxxxxxxxxxxxxxxxxxxyyyyyyyyyyyy");
        assert(a == "xxxxxxxxxxxxxxxxxxxxyyyyyyyyyyyy");
        a.release(); a.assign(""); assert(a == "");
        a.release(); a.assign("0000"); assert(a == "0000");
        a.assign("123456", 0); assert(a == "");
    }{
        string_t a("12345678"), b;
        b.assign(a, 1, 1); assert(b == "");
        b.assign(a, 0, a.len); assert(b == a);
        b.assign(a, 2, 3); assert(b == "3");
        b.release(); b.assign(a, 2, 33333); assert(b == "345678");
        b.release(); b.assign(a, 22222, 33333); assert(b == "");
        a.assign(a, 1, 5); assert(a == "2345");
        a.assign(a, 1, 1); assert(a == "");
    }{
        string_t a("12345678"), b;
        a.assign(a.dat); assert(a == "12345678");
        a.assign(a); assert(a == "12345678");
        b.assign("12345", 0); assert(b == "");
    }{
        string_t a("12345678"), b("abcdefghijklmn");
        a.assign(b, 3, 5); assert(a == "de");
        a.assign(b, 0, b.len); assert(a == b);
        a.assign(b, 0, 100); assert(a == b);
        a.assign(b, 100, 200); assert(a == "");
        a.assign(b, 4, 3); assert(a == "");
        a.assign(b, 400000, 3000); assert(a == "");
    }{
        string_t a("1234567890");
        a.assign(a.dat); assert(a == "1234567890");
        a.assign(a.dat + 3, 4); assert(a == "4567");
        a.assign(a, 3, 4); assert(a == "7");
        a.assign(a, 8, 4); assert(a == "");
    }{
        string_t a;
        a.assign('a'); assert(a == "a");
        a.assign('b'); assert(a == "b");
        a.assign("1234567890"); assert(a == "1234567890");
        a.assign(a.dat, 8); assert(a == "12345678");
        a.assign(a.dat + 3, 4); assert(a == "4567");
        a.assign(a.dat + 3, 0); assert(a == "");
        a.assign("1234567890");
        a.assign(a.dat); assert(a == "1234567890");
        a.assign(a.dat + 3); assert(a == "4567890");
    }
}

void test_string_reserve()
{
    {
        string_t a;
        a.reserve(100);
        assert(a.cap == 100);
        a.assign("123"); assert(a == "123");
        a.reserve(200);
        assert(a == "123");
        assert(a.cap == 200);
    }{
        string_t a("1234567890");
        a.reserve(0);
        assert(a == "1234567890");
        a.reserve(1);
        assert(a == "1234567890");
    }
}

void test_string_insert()
{
    {
        string_t a("12345678"), b;
        a.insert(0, "xxx"); assert(a == "xxx12345678");
        a.insert(a.len, "yyy"); assert(a == "xxx12345678yyy");
        a.insert(1, NULL); assert(a == "xxx12345678yyy");
        a.insert(1, ""); assert(a == "xxx12345678yyy");
        a.insert(7, "zzz"); assert(a == "xxx1234zzz5678yyy");
        b.insert(0, NULL); assert(b == "");
        b.insert(0, ""); assert(b == "");
        b.insert(0, "1"); assert(b == "1");
        b.release(); b.insert(100, "1"); assert(b == "1");
        b.release(); b.append("1"); assert(b == "1");
        b.prepend("a"); assert(b == "a1");
        b.append("b"); assert(b == "a1b");
        b.prepend(NULL); assert(b == "a1b");
        b.append(NULL); assert(b == "a1b");
        b.append('8', 3); assert(b == "a1b888");
        b.prepend('8', 3); assert(b == "888a1b888");
        b.insert(2, '7', 5); assert(b == "88777778a1b888");
        b.insert(b.length(), 'x', 1); assert(b == "88777778a1b888x");
        b.insert(b.length(), 'x', 0); assert(b == "88777778a1b888x");
    }{
        string_t a("12345678"), b("xyz");
        a.insert(111111, b); assert(a == "12345678xyz");
        a.insert(a.len, b); assert(a == "12345678xyzxyz");
        a.insert(a.len, b, 1, 2); assert(a == "12345678xyzxyzy");
        a.insert(a.len, b, 1, 2000); assert(a == "12345678xyzxyzyyz");
        a.insert(1111111, b, 0, 2000); assert(a == "12345678xyzxyzyyzxyz");
        a.insert(0, "abcdefg", 3); assert(a == "abc12345678xyzxyzyyzxyz");
        a.insert(3, "---", 3); assert(a == "abc---12345678xyzxyzyyzxyz");
    }{
        string_t b("xyz"), c("+-*/%");
        b.prepend(c); assert(b == "+-*/%xyz");
        b.prepend(c, 0, c.len); assert(b == "+-*/%+-*/%xyz");
        b.prepend(c, 0, 1); assert(b == "++-*/%+-*/%xyz");
        b.prepend(c, 4, 3); assert(b == "++-*/%+-*/%xyz");
        b.prepend(c, 4, 30000); assert(b == "%++-*/%+-*/%xyz");
        b.prepend("OKOK", 3); assert(b == "OKO%++-*/%+-*/%xyz");
        b.prepend("OKOK", 4); assert(b == "OKOKOKO%++-*/%+-*/%xyz");
        b.prepend("OKOK", 0); assert(b == "OKOKOKO%++-*/%+-*/%xyz");
        b.prepend((const char*)NULL, 10); assert(b == "OKOKOKO%++-*/%+-*/%xyz");
        b.prepend(string_t()); assert(b == "OKOKOKO%++-*/%+-*/%xyz");
    }{
        string_t b("xyz"), c("+-*/%");
        b.append(c); assert(b == "xyz+-*/%");
        b.append(c, 0, c.len); assert(b == "xyz+-*/%+-*/%");
        b.append(c, 0, 1); assert(b == "xyz+-*/%+-*/%+");
        b.append(c, 4, 3); assert(b == "xyz+-*/%+-*/%+");
        b.append(c, 4, 30000); assert(b == "xyz+-*/%+-*/%+%");
        b.append("OKOK", 3); assert(b == "xyz+-*/%+-*/%+%OKO");
        b.append("OKOK", 4); assert(b == "xyz+-*/%+-*/%+%OKOOKOK");
        b.append("OKOK", 0); assert(b == "xyz+-*/%+-*/%+%OKOOKOK");
        b.append((const char*)NULL, 10); assert(b == "xyz+-*/%+-*/%+%OKOOKOK");
        b.append(string_t()); assert(b == "xyz+-*/%+-*/%+%OKOOKOK");
    }{
        string_t a("abcdefghijklmnopqrst");
        a.append(a.dat, 3); assert(a == "abcdefghijklmnopqrstabc");
        a.append(a.dat + 3, 3); assert(a == "abcdefghijklmnopqrstabcdef");
        a.prepend(a.dat, 3); assert(a == "abcabcdefghijklmnopqrstabcdef");
        a.prepend(a.dat + 8, 3); assert(a == "fghabcabcdefghijklmnopqrstabcdef");
        a.insert(3, a.dat + 5, 5); assert(a == "fghcabcdabcabcdefghijklmnopqrstabcdef");
        a.insert(a.len, a.dat, 3); assert(a == "fghcabcdabcabcdefghijklmnopqrstabcdeffgh");
        a.insert(a.len, a.dat, a.len); assert(a == "fghcabcdabcabcdefghijklmnopqrstabcdeffghfghcabcdabcabcdefghijklmnopqrstabcdeffgh");
    }{
        string_t a("1234567890");
        a.append(a.dat + 5); assert(a == "123456789067890");
        a.append(a.dat + a.len); assert(a == "123456789067890");
    }
}

void test_string_remove()
{
    {
        string_t a("1234"), b("abcdefg"), c("xyz"), d;
        a.remove(0); assert(a == "234");
        a.remove(0); assert(a == "34");
        a.remove(0); assert(a == "4");
        a.remove(0); assert(a == "");
        b.remove(6); assert(b == "abcdef");
        b.remove(5); assert(b == "abcde");
        b.remove(4); assert(b == "abcd");
        b.remove(3); assert(b == "abc");
        b.remove(2); assert(b == "ab");
        b.remove(1); assert(b == "a");
        b.remove(0); assert(b == "");
        c.remove(1); assert(c == "xz");
        d.remove(0); assert(d == "");
        d.remove(1); assert(d == "");
    }{
        string_t a("1234567890");
        a.remove(6, a.len); assert(a == "123456");
        a.remove(2, 3); assert(a == "12456");
        a.remove(3, 2); assert(a == "12456");
        a.remove(3, 3); assert(a == "12456");
        a.remove(3, 33333); assert(a == "124");
        a.remove(3333, 33333); assert(a == "124");
    }{
        string_t a("1234567890");
        a.remove_to_begin(0); assert(a == "234567890");
        a.remove_to_begin(2); assert(a == "567890");
        a.remove_to_begin(a.len); assert(a == "");
        string_t b("1234567890");
        b.remove_to_begin(1000);
        assert(b == "");
    }{
        string_t a("1234567890");
        a.remove_to_end(5); assert(a == "12345");
        a.remove_to_end(2); assert(a == "12");
        a.remove_to_end(a.len); assert(a == "12");
        a.remove_to_end(0); assert(a == "");
        string_t b("1234567890");
        b.remove_to_end(1000);
        assert(b == "1234567890");
    }
}

void test_string_strip()
{
    {
        string_t a, c("\t  a\r\n"), d("  \r\t    ");
        a.strip(); assert(a.empty());
        c.strip(); assert(c == "a");
        d.strip(); assert(d == "");
    }{
        string_t a, c("\t  a\r\n"), d("  \r\t    ");
        a.strip_left(); assert(a == "");
        c.strip_left(); assert(c == "a\r\n");
        d.strip_left(); assert(d == "");
    }{
        string_t a, c("\t  a\r\n"), d("  \r\t    ");
        a.strip_right(); assert(a == "");
        c.strip_right();
        assert(c == "\t  a");
        d.strip_right(); assert(d == "");
    }{
        string_t a("+--+111---+"), b("+++---"), c;
        assert(a.pos_not_chars(0, "+-") == 4);
        assert(a.pos_not_chars(1, "+-") == 4);
        assert(a.pos_not_chars(2, "+-") == 4);
        assert(a.pos_not_chars(4, "+-") == 4);
        assert(a.pos_not_chars(5, "+-") == 5);
        assert(a.pos_not_chars(6, "+-") == 6);
        assert(a.pos_not_chars(7, "+-") == string_t::npos);
        assert(a.pos_not_chars(8, "+-") == string_t::npos);
        assert(a.pos_not_chars(2, "") == 2);
        assert(a.rpos_not_chars(a.last_pos(), "+-") == 6);
        assert(a.rpos_not_chars(9, "+-") == 6);
        assert(a.rpos_not_chars(8, "+-") == 6);
        assert(a.rpos_not_chars(7, "+-") == 6);
        assert(a.rpos_not_chars(6, "+-") == 6);
        assert(a.rpos_not_chars(5, "+-") == 5);
        assert(a.rpos_not_chars(4, "+-") == 4);
        assert(a.rpos_not_chars(3, "+-") == string_t::npos);
        assert(a.rpos_not_chars(2, "+-") == string_t::npos);
        assert(a.rpos_not_chars(2, "") == 2);
    }{
        string_t a("111222"), b("111"), c;
        assert(a.starts_with("111"));
        assert(a.starts_with("111222"));
        assert(!a.starts_with("1112223"));
        assert(!a.starts_with("@@@"));
        assert(!a.ends_with("111"));
        assert(a.ends_with("222"));
        assert(a.ends_with("111222"));
        assert(!a.ends_with("0111222"));
        assert(a.starts_with(string_t("111")));
        assert(a.starts_with(string_t("111222")));
        assert(!a.starts_with(string_t("1112223")));
        assert(!a.starts_with(string_t("@@@")));
        assert(!a.ends_with(string_t("111")));
        assert(a.ends_with(string_t("222")));
        assert(a.ends_with(string_t("111222")));
        assert(!a.ends_with(string_t("0111222")));
        assert(a.starts_with(1, "1122"));
        assert(a.ends_with(4, "1122"));
        assert(!a.starts_with(""));
        assert(!c.starts_with(""));
        assert(!a.starts_with(NULL));
        assert(!c.starts_with(NULL));
        assert(!a.ends_with(""));
        assert(!a.ends_with("123"));
        assert(!c.ends_with(""));
        assert(!c.ends_with("123"));
        assert(!a.ends_with(NULL));
        assert(!c.ends_with(NULL));
    }
}

void test_string_load()
{
    {
        NN a;
        const char* s = "  -0x123, 456";
        assert(!load(a, s, 0));
        format_t fmt;
        fmt.set_group_separator(", ");
        assert(load(a, s, 0, &fmt));
        assert(a == NN("-123456", 16));
        assert(!load(a, s, 10, &fmt));
        assert(load(a, "12345678", 10)); assert(a == NN(12345678));
        assert(load(a, "0x12345678", 16)); assert(a == NN(0x12345678));
        assert(load(a, "01234567", 8)); assert(a == NN(01234567));
        assert(load(a, "01234567", 0)); assert(a == NN(01234567));
        assert(load(a, "-0x-12345678", 16)); assert(a == NN(0x12345678));
        assert(load(a, "--0x-66666", 16)); assert(a == NN(-0x66666));
        assert(!load(a, "--0x111111", 2));
        assert(!load(a, "111102", 2));
        load(a, "+0x888", 16); assert(a == NN(0x888));
        load(a, "0x+999", 16); assert(a == NN(0x999));
        load(a, "-0x11 11", 16); assert(a == NN(-0x1111));
        load(a, "0x-2222", 16); assert(a == NN(-0x2222));
        load(a, "\t0b11 11110001", 2); assert(a == NN("1111110001", 2));
        assert(load(a, "123456712345671234567", 0, &fmt)); assert(a == NN("123456712345671234567"));
        assert(load(a, "0123456712345671234567", 0, &fmt)); assert(a == NN("0123456712345671234567", 8));
        assert(load(a, "+123456712345671234567", 0, &fmt)); assert(a == NN("123456712345671234567"));
    }{
        NN a;
        const char* s = "";
        assert(load(a, s)); assert(a.is_zero());
        assert(load(a, NULL)); assert(a.is_zero());
        format_t fmt(EMPTY_AS_ERROR);
        assert(!load(a, s, 0, &fmt));
        assert(!load(a, NULL, 0, &fmt));
    }{
        NN a; string_t s;
        set_leading(0, "b0:"); set_leading(1, "b1:"); // ineffective
        set_leading(2, "0b"); set_leading(3, "b3:"); set_leading(4, "b4:"); set_leading(5, "b5:");
        set_leading(6, "b6:"); set_leading(7, "b7:"); set_leading(8, "0"); set_leading(9, "b9:");
        set_leading(10, "b10:"); set_leading(11, "b11:"); set_leading(12, "b12:"); set_leading(13, "b13:");
        set_leading(14, "b14:"); set_leading(15, "b15:"); set_leading(16, "b16:"); set_leading(17, "b17:");
        set_leading(18, "b18:"); set_leading(19, "b19:"); set_leading(20, "b20:"); set_leading(21, "b21:");
        set_leading(22, "b22:"); set_leading(23, "b23:"); set_leading(24, "b24:"); set_leading(25, "b25:");
        set_leading(26, "b26:"); set_leading(27, "b27:"); set_leading(28, "b28:"); set_leading(29, "b29:");
        set_leading(30, "b30:"); set_leading(31, "b31:"); set_leading(32, "b32:"); set_leading(33, "b33:");
        set_leading(34, "b34:"); set_leading(35, "b35:"); set_leading(36, "b36:");
        set_leading(37, "b37:"); // ineffective
        assert(load(a, "b16:abcde")); assert(a == 0xabcde);
        assert(load(a, "b36:-uvwxyz", 0)); assert(a == NN("-uvwxyz", 36));
        format_t f(SHOW_LEADING | SIGN_RIGHT_LEADING);
        f.dump(a, 36, s); assert(s == "b36:-uvwxyz");
        set_leading(36, NULL); assert(!load(a, "b36:-uvwxyz", 0));
        f.dump(a, 36, s); assert(s == "-uvwxyz");
        assert(load(a, "b35:-uvwxxx", 0)); assert(a == NN("-uvwxxx", 35));
        f.dump(a, 35, s); assert(s == "b35:-uvwxxx");
        set_leading(35, NULL); assert(!load(a, "b35:-uvwxxx", 0));
        f.dump(a, 35, s); assert(s == "-uvwxxx");
        assert(load(a, "b18:-12345", 0)); assert(a == NN("-12345", 18));
        f.dump(a, 18, s); assert(s == "b18:-12345");
        set_leading(18, NULL); assert(!load(a, "b18:-12345", 0));
        f.dump(a, 18, s); assert(s == "-12345");
        assert(load(a, "b17:-12345", 0)); assert(a == NN("-12345", 17));
        assert(load(a, "b19:-12345", 0)); assert(a == NN("-12345", 19));
    }{
        reset_leading();
        NN a;
        assert(load(a, NULL, 0)); assert(a.is_zero());
        assert(load(a, "", 0)); assert(a.is_zero());
        assert(load(a, "-", 0)); assert(a.is_zero());
        assert(load(a, "+", 0)); assert(a.is_zero());
        assert(load(a, "0x", 0)); assert(a.is_zero());
        assert(load(a, "0", 0)); assert(a.is_zero());
        assert(load(a, "0b", 0)); assert(a.is_zero());
        assert(load(a, "- 0x ", 0)); assert(a.is_zero());
        format_t f(EMPTY_AS_ERROR);
        assert(!load(a, NULL, 0, &f));
        assert(!load(a, "", 0, &f));
        assert(!load(a, "-", 0, &f));
        assert(!load(a, "+", 0, &f));
        assert(!load(a, "0x", 0, &f));
        assert(!load(a, "0", 0, &f));
        assert(!load(a, "0b", 0, &f));
        assert(!load(a, " - 0x", 0, &f));
        assert(load(a, " - -0x+-456", 0, &f)); assert(a == -0x456);
        assert(load(a, "- -2", 0, &f)); assert(a == 2);
        assert(load(a, "+-1", 0, &f)); assert(a == -1);
        f.set(MULTISIGN_AS_ERROR);
        assert(!load(a, " - -0x+-456", 0, &f));
        assert(!load(a, "- -2", 0, &f));
        assert(!load(a, "+-1", 0, &f));
    }{
        NN a;
        assert(load(a, " - 0 X123", 0)); assert(a == -291);
        assert(load(a, " - 0X123", 16)); assert(a == -291);
        assert(!load(a, " - 0X123", 8)); assert(!load(a, " - 0X123", 10));
        assert(!load(a, "123", 37));
    }
}

void test_string_dump()
{
    {
        reset_leading();
        NN a, b, c(-123);
        string_t s;
        format_t f(SHOW_LEADING | SIGN_RIGHT_LEADING);
        set_leading(32, "0xx");
        assert(load(a, "0xx12345 ", 0)); assert(a == NN("12345", 32));
        assert(f.dump(a, 32, s) == "0xx12345");
        f.set(SHOW_POS); assert(f.dump(b, 16, s) == "0x0");
        f.set(ZERO_POS); assert(f.dump(b, 16, s) == "0x+0");
        f.set(ZERO_NEG); assert(f.dump(b, 16, s) == "0x-0");
        f.set(ZERO_NO_LEADING); f.dump(b, 16, s); assert(s == "-0");
    }{
        reset_leading();
        NN a(123), b, c(-123);
        string_t s;
        format_t f2, f10, f16, f33;
        f10.dump(a, 10, s); assert(s == "123");
        f10.dump(c, 10, s); assert(s == "-123");
        f10.set(SHOW_POS);
        f10.dump(a, 10, s); assert(s == "+123");
        f10.dump(c, 10, s); assert(s == "-123");
        f10.set(SHOW_LEADING);
        f10.dump(a, 10, s); assert(s == "+123");
        f10.dump(c, 10, s); assert(s == "-123");
        f16.dump(a, 16, s); assert(s == "7b");
        f16.dump(c, 16, s); assert(s == "-7b");
        f16.set(SHOW_LEADING);
        f16.dump(a, 16, s); assert(s == "0x7b");
        f16.dump(c, 16, s); assert(s == "-0x7b");
        f16.set(SHOW_POS);
        f16.dump(a, 16, s); assert(s == "+0x7b");
        f16.dump(c, 16, s); assert(s == "-0x7b");
        f16.set(SIGN_RIGHT_LEADING);
        f16.dump(a, 16, s); assert(s == "0x+7b");
        f16.dump(c, 16, s); assert(s == "0x-7b");
        f16.set_group_size(1);
        f16.set_group_separator(" ");
        f16.dump(c, 16, s); assert(s == "0x-7 b");
        f16.set_group_size(2); f16.dump(c, 16, s); assert(s == "0x-7b");
        f16.set_group_size(3); f16.dump(c, 16, s); assert(s == "0x-7b");
        f33.dump(a, 33, s); assert(s == "3o");
        assert(get_leading(33) == NO_LEADING);
        set_leading(33, "base(33):");
        assert(get_leading(33) == string_t("base(33):"));

        f33.dump(a, 33, s); assert(s == "3o");
        f33.set(SHOW_LEADING);
        f33.dump(a, 33, s); assert(s == "base(33):3o");

        f33.set(SIGN_RIGHT_LEADING);
        f33.dump(c, 33, s); assert(s == "base(33):-3o");
        f33.set(UPPER_CASE);
        f33.dump(c, 33, s); assert(s == "base(33):-3O");
        f33.set(UPPER_LEADING);
        f33.dump(c, 33, s); assert(s == "BASE(33):-3O");
        set_leading(33, "");
        assert(get_leading(33) == NO_LEADING);
        set_leading(33, NULL);
        assert(get_leading(33) == NO_LEADING);

        f10.set(NO_FLAGS);
        f10.set_group_size(4);
        f10.set_group_separator(", ");
        f10.dump(a, 10, s); assert(s == "123");
        a.sqr().sqr();
        f10.dump(a, 10, s); assert(s == "2, 2888, 6641");
        f10.set_group_size(1); f10.dump(a, 10, s); assert(s == "2, 2, 8, 8, 8, 6, 6, 4, 1");
        f10.dump(b, 10, s); assert(s == "0");
        f10.set(GROUP_COMPLETE);
        f10.set_group_size(1); f10.dump(a, 10, s); assert(s == "2, 2, 8, 8, 8, 6, 6, 4, 1");
        f10.set_group_size(2); f10.dump(a, 10, s); assert(s == "02, 28, 88, 66, 41");
        f10.set_group_filler('*');
        f10.set_group_size(2); f10.dump(a, 10, s); assert(s == "*2, 28, 88, 66, 41");
        f10.dump(b, 10, s); assert(s == "*0");

        f2.set_group_separator(" ");
        f2.dump(a, 2, s); assert(s == "1101101001001000100001110001");
        f2.set_group_size(8); f2.dump(a, 2, s); assert(s == "1101 10100100 10001000 01110001");
        f2.set(GROUP_COMPLETE); f2.dump(a, 2, s); assert(s == "00001101 10100100 10001000 01110001");
        f2.set(SHOW_LEADING); f2.dump(a, 2, s); assert(s == "0b00001101 10100100 10001000 01110001");
        f2.set_group_separator("");
        f2.dump(a, 2, s); assert(s == "0b00001101101001001000100001110001");
        f2.clear(GROUP_COMPLETE);
        f2.set_group_size(8000);
        f2.dump(a, 2, s); assert(s == "0b1101101001001000100001110001");
    }{
        NN a("1234567890abcdef1234567890abcdef1234", 16);
        NN b("1234567890abcdef1234567890abcdef", 16);
        string_t res;
        format_t fmt;
        fmt.set_group_size(4);
        fmt.set_group_separator(" ");
        fmt.set_line_group_count(4);
        fmt.set_line_separator("\n");
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef\n1234 5678 90ab cdef\n1234");
        fmt.dump(b, 16, res); assert(res == "1234 5678 90ab cdef\n1234 5678 90ab cdef");
        fmt.set_line_group_count(1);
        fmt.dump(a, 16, res); assert(res == "1234\n5678\n90ab\ncdef\n1234\n5678\n90ab\ncdef\n1234");
        b.assign("123", 16);
        fmt.dump(b, 16, res); assert(res == "123");
        fmt.set(GROUP_COMPLETE);
        fmt.dump(b, 16, res); assert(res == "0123");
        b.assign("12345", 16);
        fmt.dump(b, 16, res); assert(res == "0001\n2345");
        fmt.set_line_group_count(2);
        fmt.dump(b, 16, res); assert(res == "0001 2345");
        b.assign("123456789", 16);
        fmt.dump(b, 16, res); assert(res == "0001 2345\n6789");
        fmt.set_line_group_count(0);
        fmt.dump(b, 16, res); assert(res == "0001 2345 6789");
        fmt.clear(GROUP_COMPLETE);
        fmt.dump(b, 16, res); assert(res == "1 2345 6789");
        fmt.set_group_size(1);
        fmt.set_line_group_count(1);
        fmt.dump(b, 16, res); assert(res == "1\n2\n3\n4\n5\n6\n7\n8\n9");
        fmt.set_group_size(4);
        fmt.set_line_group_count(1);
        fmt.set_group_separator(NULL);
        fmt.dump(b, 16, res); assert(res == "1\n2345\n6789");
        fmt.set(GROUP_FROM_MSB);
        fmt.dump(b, 16, res); assert(res == "1234\n5678\n9");
        fmt.set_group_size(1);
        fmt.dump(b, 16, res); assert(res == "1\n2\n3\n4\n5\n6\n7\n8\n9");
        b.sub(9).div(16);
        fmt.dump(b, 16, res); assert(res == "1\n2\n3\n4\n5\n6\n7\n8");
        b.set_zero(); fmt.dump(b, 16, res); assert(res == "0");
        b.release(); fmt.dump(b, 16, res); assert(res == "0");
    }{
        NN a("1234567890abcdef1234567890abcdef1234", 16);
        string_t res;
        format_t fmt;
        fmt.set_group_size(4);
        fmt.set_line_group_count(4);
        fmt.dump(a, 16, res); assert(res == "1234567890abcdef\n1234567890abcdef\n1234");
        fmt.set_group_size(400);
        fmt.dump(a, 16, res); assert(res == "1234567890abcdef1234567890abcdef1234");
        fmt.set_group_size(4);
        fmt.set_group_separator(" ");
        fmt.set_line_group_count(0);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab cdef 1234");
        fmt.set_line_group_count(1110);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab cdef 1234");
        fmt.set_line_group_count(2);
        fmt.dump(a, 16, res); assert(res == "1234 5678\n90ab cdef\n1234 5678\n90ab cdef\n1234");
        a.sub(4).div(16);
        fmt.dump(a, 16, res); assert(res == "123 4567\n890a bcde\nf123 4567\n890a bcde\nf123");
        fmt.set(GROUP_FROM_MSB);
        fmt.dump(a, 16, res); assert(res == "1234 5678\n90ab cdef\n1234 5678\n90ab cdef\n123");
        fmt.set_line_group_count(100);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab cdef 123");
        fmt.set_line_group_count(9);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab cdef 123");
        fmt.set_line_group_count(8);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab cdef\n123");
        fmt.set_line_group_count(7);
        fmt.dump(a, 16, res); assert(res == "1234 5678 90ab cdef 1234 5678 90ab\ncdef 123");
        fmt.set_group_size(1000); fmt.dump(a, 16, res); assert(res == "1234567890abcdef1234567890abcdef123");
        fmt.set_group_size(35); fmt.dump(a, 16, res); assert(res == "1234567890abcdef1234567890abcdef123");
        fmt.set_group_size(34); fmt.dump(a, 16, res); assert(res == "1234567890abcdef1234567890abcdef12 3");
        a.set_zero(); fmt.dump(a, 16, res); assert(res == "0");
        a.release(); fmt.dump(a, 16, res); assert(res == "0");
    }{
        reset_leading();
        set_leading(35, NULL);
        set_leading(36, NULL);
        set_leading(37, NULL);
    }{
        number_t a, b(123), c(1234), d(12345);
        format_t f;
        string_t s;
        f.set_group_size(4);
        f.set_group_separator(", ");
        f.dump(a, 10, s); assert(s == "0");
        f.dump(b, 10, s); assert(s == "123");
        f.dump(c, 10, s); assert(s == "1234");
        f.dump(d, 10, s); assert(s == "1, 2345");
        f.set(GROUP_FROM_MSB);
        f.dump(a, 10, s); assert(s == "0");
        f.dump(b, 10, s); assert(s == "123");
        f.dump(c, 10, s); assert(s == "1234");
        f.dump(d, 10, s); assert(s == "1234, 5");
    }
}

void test_string_find()
{
    {
        string_t s("abcdefg");
        assert(s.find('c') == 2);
        assert(s.find("c") == 2);
        assert(s.find("x") == string_t::npos);
        assert(s.find(4, 'c') == string_t::npos);
        assert(s.find(4, "c") == string_t::npos);
        assert(s.find(4, "x") == string_t::npos);
        assert(s.find("def") == 3);
        assert(s.find("defg") == 3);
        string_t t("bcde");
        assert(s.find(t) == 1);
        assert(s.find(1, t) == 1);
        assert(s.find(2, t) == string_t::npos);
    }{
        string_t a("abcdefghijklmn"), b;
        assert(a.contains('a'));
        assert(a.contains("a"));
        assert(a.contains("ijklmn"));
        assert(a.contains("abcdefghijklmn"));
        assert(!a.contains("ac"));
        assert(!b.contains('a'));
        assert(!b.contains("a"));
        assert(!b.contains("ijklmn"));
        assert(!b.contains("abcdefghijklmn"));
        assert(!b.contains("ac"));
    }
}

void test_string()
{
    test_string_assignment();
    test_string_reserve();
    test_string_insert();
    test_string_remove();
    test_string_strip();
    test_string_find();
    test_string_load();
    test_string_dump();

    string_t a("abcd");
    string_t b("ABCD");
    assert(a != b);
    assert(a.valid());
    assert(b.to_lower() == "abcd");
    assert(a.to_lower() == b.to_lower());
    assert(a.to_upper() == "ABCD");
    assert(a.to_upper() == b.to_upper());
    string_t c, d("");
    assert(d.valid()); assert(!c.valid());
    assert(cmp(c, d) == 0);
    assert(c == d);

    string_t e("1234567890", 3); assert(e == "123");
    string_t f("1234567890", 0); assert(f == "");
    string_t g(NULL, 123); assert(!g.valid()); assert(g == "");

    string_t h(e); assert(h == e && h == "123");
    string_t i(g); assert(i == g && !i.valid());

    string_t o("abcd");
    assert(o[0] == 'a'); assert(o[1] == 'b');
    o[0] = '0'; assert(o == "0bcd");

    assert("abcd" < string_t("abce"));
    assert(string_t("abce") > "abcd");
    assert("abcd" <= string_t("abce"));
    assert(string_t("abce") >= "abcd");
    assert("abcd" != string_t("abce"));
    assert(string_t("abce") != "abcd");
    assert("abcd" < string_t("abcde"));
    assert(string_t("abcde") > "abcd");
    assert(string_t("abcd") < string_t("abcde"));
    assert(string_t("abcde") > string_t("abcd"));
    assert("abcd" <= string_t("abcde"));
    assert(string_t("abcde") >= "abcd");
    assert(string_t("abcd") <= string_t("abcde"));
    assert(string_t("abcde") >= string_t("abcd"));
    assert("abcd" != string_t("abcde"));
    assert(string_t("abcde") != "abcd");

    {
        assert(!check("", 2));
        assert(!check("+", 10));
        assert(check("-", 10) == 1);  // "-" indicates 0
        assert(check("+123456", 10) == 0);
        assert(check("-123456", 10) == 7);
        assert(check("-123456", 3, 10) == 3);
        assert(check("-123456", 11));
        assert(check("-123456", 12) == 7);
        assert(check("-123456", 0, 12) == 0);
        assert(!check("-a12345", 8));
        assert(!check("-a12345", 10));
        assert(check("-a12345", 11));

        assert(check(string_t("+123456"), 10) == 0);
        assert(check(string_t("-123456"), 10) == 7);
        assert(check(string_t("-123456"), 0, 3, 10) == 3);
        assert(check(string_t("-123456"), 11));
        assert(check(string_t("-123456"), 12) == 7);
        assert(check(string_t("-123456"), 0, 0, 12) == 0);
        assert(!check(string_t("-a12345"), 8));
        assert(!check(string_t("-a12345"), 10));
        assert(check(string_t("-a12345"), 11));

        assert(!check(string_t("-a12345"), 100, 0, 11));
        assert(!check(string_t("-a12345"), 100, 1000, 11));
        assert(check(string_t("-a12345"), 3, 1000, 11) == 4);
    }
#ifndef NO_STL_SUPPORT
    {
        using namespace std;
        number_t a("2348975234572304985723890457");
        number_t b("-2348975234572304985723890457");
        ostringstream oss;
        oss << a; assert(oss.str() == "2348975234572304985723890457");
        oss.str("");
        oss << b; assert(oss.str() == "-2348975234572304985723890457");
        oss.str("");
        oss << setw(32) << setfill('*') << left << hex << showpos << showbase << a;
        assert(oss.str() == "+0x79706da9f34a4acc209eb19******");
        oss.str(""); oss << uppercase << a; assert(oss.str() == "+0x79706DA9F34A4ACC209EB19");
        oss.str("");
        oss << setw(32) << setfill('*') << left << hex << showpos << showbase << b;
        assert(oss.str() == "-0x79706DA9F34A4ACC209EB19******");
        oss.str("");
        oss << nouppercase << a(33); assert(oss.str() == "12wifbtceu89thjubk6");
        oss << uppercase << a(33); assert(oss.str() == "12wifbtceu89thjubk612WIFBTCEU89THJUBK6");
    }
#endif
    {
        string_t a("1234567890");
        string_t b(a, 3, 6);
        assert(b == "456");
        string_t c(a, 3, 600); assert(c == "4567890");
        string_t d(a, 300, 600); assert(d == "");
        string_t e(a, 6, 3); assert(e == "");
        string_t f;
        string_t g(f, 0, 3); assert(g == "");

        string_t h('a'); assert(h == "a");
        string_t j(0); assert(j == "");
    }{
        string_t a(1234); assert(a.capacity() == 1234);
        string_t b(NULL); assert(b == "");
        string_t c(NULL, 123); assert(c == "");
        string_t d(0); assert(d == "");
        number_t x(123);
        string_t e(x);  assert(e.capacity() == 123);
    }

    reset_leading();
}

void test_add_small()
{
    {
        NN a;
        a.add_unit(32452); a.add_unit(5648); a.add_unit(57); a.add_unit(4);
        a.add_unit(56756); a.add_unit(332); a.add_unit(8976); a.add_unit(993);
        a.add_unit(4321); a.add_unit(8907); a.add_unit(0);
        assert(a == 118446);

        a.set_neg();
        a.add_unit(32452); a.add_unit(5648); a.add_unit(57); a.add_unit(4);
        a.add_unit(56756); a.add_unit(332); a.add_unit(8976); a.add_unit(993);
        a.add_unit(4321); a.add_unit(8907); a.add_unit(0);
        assert(a == 0);
        add_unit(a, 123, a); assert(a == 123);
        add_unit(a, 123, a); assert(a == 246);

        a.release();
        a.bits_reserve(1111);
        a.add_unit(32452); a.add_unit(5648); a.add_unit(57); a.add_unit(4);
        a.add_unit(56756); a.add_unit(332); a.add_unit(8976); a.add_unit(993);
        a.add_unit(4321); a.add_unit(8907); a.add_unit(0);
        assert(a == 118446);
        a.set_neg();
        a.add_unit(32452); a.add_unit(5648); a.add_unit(57); a.add_unit(4);
        a.add_unit(56756); a.add_unit(332); a.add_unit(8976); a.add_unit(993);
        a.add_unit(4321); a.add_unit(8907); a.add_unit(0);
        assert(a == 0);

        NN b(-456); b.add_unit(1000); assert(b == 544);
        NN c(-123); c.add_unit(123); assert(c == 0);
        add_unit(a, 123, b); assert(b == 123);
    }{
        NN a;
        a.bit_set_one(1111);
        a.set_zero();
        a.add_unit(0);
        assert(a == 0);
        a.add_unit(1);
        a.add_unit(0);
        assert(a == 1);
    }{
        NN a("1234567890"), b("-1234567890"), c, d(-1234);
        a.add_unit(1234); assert(a == 1234569124ULL);
        b.add_unit(1234); assert(b == -1234566656LL);
        c.add_unit(1234); assert(c == 1234);
        d.add_unit(1234); assert(d == 0);
    }{
        NN a;
        for (unit_t i = 0; i < 1000; i++) a.add_unit(i);
        NN b;
        b.bits_reserve(1000);
        for (unit_t i = 0; i < 1000; i++) b.add_unit(i);
        assert(a == b);
        assert(a == 499500);
    }{
        NN a("12345678901234567890");
        a.add(65535); assert(a == NN("12345678901234633425"));
        a.add(65536); assert(a == NN("12345678901234698961"));
        a.set_neg();
        a.add(65535); assert(a == NN("-12345678901234633426"));
        a.add(65536); assert(a == NN("-12345678901234567890"));

        NN b("FFFFFFFFFFFFFFFF", 16);
        b.add(65535); assert(b == NN("18446744073709617150"));
        b.add(0xffffffff); assert(b == NN("18446744078004584445"));
        b.set_neg();
        b.add(65535); assert(b == NN("-18446744078004518910"));
        b.add(65536); assert(b == NN("-18446744078004453374"));

        NN c(1234);
        c.add(0xffffffff); assert(c == NN("4294968529"));
        NN d("ffffffffbbee", 16);
        d.add(0xffff); assert(d(16) == "100000000bbed");
        NN e("ffffffffffffbbee", 16);
        e.add(0xffff); assert(e(16) == "1000000000000bbed");       
        NN _e_("ffffffffffffbbee", 16);
        _e_.bits_reserve(300);
        _e_.add(0xffff); assert(_e_(16) == "1000000000000bbed");
        NN f("aaaaaaaaffffffffffffbbee", 16);
        f.add(0xffff); assert(f(16) == "aaaaaaab000000000000bbed");
        NN g("aaaabbbbffffffff", 16);
        g.add(0xffff); assert(g(16) == "aaaabbbc0000fffe");
        NN h("aaaaffffffffffffffff", 16);
        h.add(0xffff); assert(h(16) == "aaab000000000000fffe");
        NN i("aaaaffffffffffffffffffff", 16);
        i.add(0xffff); assert(i(16) == "aaab0000000000000000fffe");
        NN j("aaaaffffffffffffffffffff", 16);
        j.bits_reserve(300);
        j.add(0xffff); assert(j(16) == "aaab0000000000000000fffe");
    }{
        NN a(1234);
        a.add_sword(-1234); assert(a == 0);
        a.add_sword(-1234); assert(a == -1234);
        a.add_sword(-1234); assert(a == -2468);
        a.add_sword(1234); assert(a == -1234);
        a.add_sword(1234); assert(a == 0);
        a.add_sword(1234); assert(a == 1234);
        a.add_sword(1234); assert(a == 2468);
        a.add_sword(1111234); assert(a == 1113702);
        a.assign(1234567); a.add_sword(-1234); assert(a == 1233333);
        a.assign(1235); a.add_sword(-1234); assert(a == 1);
        a.assign(1230); a.add_sword(-1234); assert(a == -4);
        a.set_zero(); a.add_word(12345678); assert(a == 12345678);
        a.set_zero(); a.add_word(123); assert(a == 123);
        a.assign(1324567); a.add_word(123); assert(a == 1324690);
        a.assign(-1324567); a.add_word(123); assert(a == -1324444);
        a.assign(-1324567); a.add_word(1324568); assert(a == 1);
    }{
        NN a("FFFFFFFF", 16); a.add_unit(1); assert(a == NN("100000000", 16));
        NN b("FFFFFFFFF", 16); b.add_unit(1); assert(b == NN("1000000000", 16));
        NN c("FFFFFFFFFF", 16); c.add_unit(1); assert(c == NN("10000000000", 16));
        NN d("FFFFFFFFFFF", 16); d.add_unit(1); assert(d == NN("100000000000", 16));
        NN e("FFFFFFFFFFFF", 16); e.add_unit(1); assert(e == NN("1000000000000", 16));
        NN f("FFFFFFFFFFFFF", 16); f.add_unit(1); assert(f == NN("10000000000000", 16));
    }{
        NN a("FFFFFFFF", 16); a.add_word(1); assert(a == NN("100000000", 16));
        NN b("FFFFFFFFF", 16); b.add_sword(1); assert(b == NN("1000000000", 16));
        NN c("FFFFFFFFFF", 16); c.add_word(1); assert(c == NN("10000000000", 16));
        NN d("FFFFFFFFFFF", 16); d.add_sword(1); assert(d == NN("100000000000", 16));
        NN e("FFFFFFFFFFFF", 16); e.add_word(1); assert(e == NN("1000000000000", 16));
        NN f("FFFFFFFFFFFFF", 16); f.add_sword(1); assert(f == NN("10000000000000", 16));
    }
}

void test_sub_small()
{
    {
        NN a(123), b(-123);
        a.sub_unit(124); assert(a == -1);
        a.sub_unit(0); assert(a == -1);
        a.sub_unit(4); assert(a == -5);
        b.sub_unit(1); assert(b == -124);
        a.set_one(); a.shl(128); a.sub_unit(123);
        assert(a == NN("340282366920938463463374607431768211333"));
    }{
        NN a(118446);
        a.sub_unit(32452); assert(a == 85994);
        a.sub_unit(5648); assert(a == 80346);
        a.sub_unit(57); a.sub_unit(4);
        a.sub_unit(56756); a.sub_unit(332); a.sub_unit(8976); a.sub_unit(993);
        a.sub_unit(4321);
        a.sub_unit(8907);
        a.sub_unit(0);
        assert(a == 0);
        a.sub_unit(32452); a.sub_unit(5648); a.sub_unit(57); a.sub_unit(4);
        a.sub_unit(56756); a.sub_unit(332); a.sub_unit(8976); a.sub_unit(993);
        a.sub_unit(4321); a.sub_unit(8907); a.sub_unit(0);
        assert(a == -118446);

        NN b(456); b.sub_unit(1000); assert(b == -544);
        NN c(123); c.sub_unit(123); assert(c == 0);
        NN d(-123); d.sub_unit(123); assert(d == -246);
    }{
        NN a("1234567890"), b("-1234567890"), c, d(-1234), e(1234);
        a.sub_unit(1234); assert(a == 1234566656);
        b.sub_unit(1234); assert(b == -1234569124);
        c.sub_unit(1234); assert(c == -1234);
        d.sub_unit(1234); assert(d == -2468);
        e.sub_unit(1234); assert(e == 0);
    }{
        NN a(1234), b(-1234);
        a.sub_sword(1234); assert(a == 0);
        a.sub_sword(-1234); assert(a == 1234);
        a.sub_sword(-1234); assert(a == 2468);
        a.sub_sword(1234); assert(a == 1234);
        a.sub_sword(1234); assert(a == 0);
        a.sub_sword(1234); assert(a == -1234);
        a.sub_sword(1234); assert(a == -2468);
        a.sub_sword(1111234); assert(a == -1113702);
        a.assign(1234567); a.sub_sword(-1234); assert(a == 1235801);
        a.assign(1235); a.sub_sword(1234); assert(a == 1);
        a.assign(1230); a.sub_sword(1234); assert(a == -4);
        a.set_zero(); a.sub_word(12345678); assert(a == -12345678);
        a.set_zero(); a.sub_word(123); assert(a == -123);
        a.assign(1324567); a.sub_word(123); assert(a == 1324444);
        a.assign(-1324567); a.sub_word(123); assert(a == -1324690);
        b.sub_sword(-1234); assert(b == 0);
        b.sub_sword(-1234); assert(b == 1234);
    }{
        NN a("100000000", 16); a.sub_unit(1); assert(a == NN("FFFFFFFF", 16));
        NN b("1000000000", 16); b.sub_unit(1); assert(b == NN("FFFFFFFFF", 16));
        NN c("10000000000", 16); c.sub_unit(1); assert(c == NN("FFFFFFFFFF", 16));
        NN d("100000000000", 16); d.sub_unit(1); assert(d == NN("FFFFFFFFFFF", 16));
        NN e("1000000000000", 16); e.sub_unit(1); assert(e == NN("FFFFFFFFFFFF", 16));
        NN f("10000000000000", 16); f.sub_unit(1); assert(f == NN("FFFFFFFFFFFFF", 16));
    }{
        NN a("100000000", 16); a.sub_word(1); assert(a == NN("FFFFFFFF", 16));
        NN b("1000000000", 16); b.sub_sword(1); assert(b == NN("FFFFFFFFF", 16));
        NN c("10000000000", 16); c.sub_word(1); assert(c == NN("FFFFFFFFFF", 16));
        NN d("100000000000", 16); d.sub_sword(1); assert(d == NN("FFFFFFFFFFF", 16));
        NN e("1000000000000", 16); e.sub_word(1); assert(e == NN("FFFFFFFFFFFF", 16));
        NN f("10000000000000", 16); f.sub_sword(1); assert(f == NN("FFFFFFFFFFFFF", 16));
    }
}

void test_mul_small()
{
    {
        NN a(1);
        a.mul_unit(32452); a.mul_unit(5648); a.mul_unit(57); a.mul_unit(4);
        a.mul_unit(56756); a.mul_unit(332); a.mul_unit(8976); a.mul_unit(993);
        a.mul_unit(4321); a.mul_unit(8907); a.mul_unit(1);
        assert(a == NN("270127424034073692837664953532416"));
        a.mul_unit(0); assert(a == NN("0"));
        mul_unit(a, 12345, a); assert(a == NN("0"));
        sub_unit(a, 12345, a); assert(a == NN("-12345"));
        mul_unit(a, 12345, a); assert(a == NN("-152399025"));

        NN z;
        z.mul_unit(123); z.mul_unit(456); z.mul_unit(7890);
        assert(z.is_zero());
        mul_unit(a, 12345, z); assert(z == NN("-1881365963625"));
        mul_unit(a, 0, z); assert(z.is_zero());
    }{
        NN a("1234567890"), b("-1234567890"), c, d(-1234);
        a.mul_unit(1234); assert(a == NN("1523456776260"));
        b.mul_unit(1234); assert(b == NN("-1523456776260"));
        c.mul_unit(1234); assert(c == 0);
        d.mul_unit(1234); assert(d == -1522756);
    }{
        NN a;
        a.set_one();
        for (unit_t i = 1; i < 32; i++)
        {
            a.mul_unit(i);
        }
        NN b(1);
        b.bits_reserve(1000);
        for (unit_t i = 1; i < 32; i++)
        {
            b.mul_unit(i);
        }
        assert(a == b);
        assert(b == NN("8222838654177922817725562880000000"));
    }{
        NN a("1");
        a.mul(65538);
        assert(a(10) == "65538");
        NN b("23424623456345623452345");
        b.mul(65538);
        assert(b(10) == "1535202972081979469819786610");
#if UNITBITS == 32
        NN c("23424623456345623452345");
        c.mul(0xffffaaafafafaaaaULL);
        assert(c(10) == "432105836612120073429292480918188622354650");
#elif UNITBITS == 16
		NN c("23424623456345623452345");
        c.mul(0xabcdef00);
        assert(c(10) == "67519134650570625039039228000000");
#endif
		NN e(0);
		e.mul(0xffffaaa);
		assert(e(10) == "0");
    }{
        NN a; a.mul_sword(123); assert(a == 0);
        a.assign(1); a.mul_sword(123); assert(a == 123);
        a.assign(1); a.mul_sword(-123); assert(a == -123);
        a.assign(-1); a.mul_sword(-123); assert(a == 123);
        a.assign(-655365); a.mul_sword(-655365); assert(a == 429503283225);
        a.mul_word(65536); assert(a == 28147927169433600ULL);
        a.assign(-65536); a.mul_word(65536); assert(a == -4294967296);
    }{
        NN a, b;
        mul_unit(a, 123, b); assert(b == 0);
        mul_unit(a, 123, a); assert(a == 0);
        a.assign(1111); mul_unit(a, 0, a); assert(a == 0);
        a.release();
        mul_unit(a, 0, a); assert(a == 0);
        a.release();
        a.mul_unit(123); assert(a == 0);
        a.mul_unit(0); assert(a == 0);
        a.assign(1111); a.mul_unit(0); assert(a == 0);
    }
}

void test_div_small()
{
    {
        NN a, b;
        a.div_unit(1); assert(a == 0);
        a.div_unit(13459); assert(a == 0);
        div_unit(a, 123, a); assert(a == 0);
        div_unit(a, 123, b); assert(b == 0);
    }{
        NN a("270127424034073692837664953532416");
        a.div_unit(1); a.div_unit(32452); div_unit(a, 5648, a); a.div_unit(57); a.div_unit(4);
        a.div_unit(56756); a.div_unit(332); div_unit(a, 8976, a); a.div_unit(993);
        a.div_unit(4321); div_unit(a, 8907, a);
        assert(a == 1);
    }{
        NN a("94837582435723485723049587239057");
        assert(a.div_unit(1234) == 807); assert(a == NN("76853794518414494102957526125"));
        assert(a.div_unit(1234) == 227); assert(a == NN("62280222462248374475654397"));
        div_unit(a, 1234, a); assert(a == NN("50470196484804193254176"));
        a.div_unit(1234); assert(a == NN("40899673002272441859"));
    }{
        NN a("45633457894837582435723485546897235468972348900998918122737467723049587239057");
        assert(a.div(1234) == NN("36980111746221703756664088773822719180690720341166060067048191023540994521"));
        assert(a.div(1234567) == NN("29953912380795618023699069207116923731713807627424076673885006665123"));
        assert(a.div(370449408) == NN("80858308135818691937818049386967636162921895200934095362044"));
    }{
        NN a("45633457894837582435723485546897235468972348900998918122737467723049587239057");
        assert(a.div_word(1234) == 143); assert(a == NN("36980111746221703756664088773822719180690720341166060067048191023540994521"));
        assert(a.div_sword(-1234567) == 87780); assert(a == NN("-29953912380795618023699069207116923731713807627424076673885006665123"));
        assert(a.div_word(370449408) == 261195171); assert(a == NN("-80858308135818691937818049386967636162921895200934095362044"));
        a.assign("340282366920938463463374607431768211456");
        assert(a.div_word(65536) == 0); assert(a == NN("5192296858534827628530496329220096"));
        a.div_word(1234); a.assign(0); assert(a == 0);
        a.assign(1); a.div_word(1234); assert(a == 0);
        a.assign(0); a.div_sword(-1234); assert(a == 0);
        a.assign(1); a.div_sword(-1234); assert(a == 0);
        a.assign(437454245000ULL);
        assert(!a.absrem_unit(50));
        assert(a.absrem_unit(12) == 8);
        assert(!a.absrem_unit(20));
        assert(a.absrem_unit(12345) == 10010);
        assert(!a.absrem_unit(5));
        a.set_zero();
        assert(a.absrem_unit(12) == 0);
        assert(a.absrem_unit(12345) == 0);
        a.release();
        assert(a.absrem_unit(12) == 0);
        assert(a.absrem_unit(12345) == 0);
        assert(a.absrem_unit(UDM(12)) == 0);
        assert(a.absrem_unit(UDM(12345)) == 0);
    }{
        NN a("1234567");
        a.div_unit(0);
        a.div(int(0));
        a.div(long(0));
        a.div((long long)0);
        a.div((unsigned int)0);
        a.div((unsigned long)0);
        a.div((unsigned long long)0);
    }{
        NN a("1234567"), b;
        assert(div_unit(a, 3, b) == 1); assert(b == 411522);
        assert(div_unit(b, 3, a) == 0); assert(a == 137174);
        assert(div_unit(a, 317, a) == 230); assert(a == 432);
        assert(div_unit(a, 433, b) == 432); assert(b == 0);
        a.assign("-1234567"); b.release();
        assert(div_unit(a, 3, b) == 1); assert(b == -411522);
        assert(div_unit(b, 3, a) == 0); assert(a == -137174);
        assert(div_unit(a, 317, a) == 230); assert(a == -432);
        assert(div_unit(a, 433, b) == 432); assert(b == 0);
        a.assign(-655309);
        assert(div_unit(a, 65531, b) == 65530); assert(b == -9);
    }{
        NN a("335456123445434567097887489567346758"), b;
        assert(a.div_unit(UDM(1)) == 0); assert(a == NN("335456123445434567097887489567346758"));
        assert(a.div_unit(UDM(3)) == 2); assert(a == NN("111818707815144855699295829855782252"));
        assert(a.div_unit(UDM(377)) == 163); assert(a == NN("296601346989774153048530052667857"));
        assert(a.div_unit(UDM(1024)) == 465); assert(a == NN("289649752919701321336455129558"));
        assert(a.div_unit(UDM(32768)) == 27094); assert(a == NN("8839408963613931925550998"));
        assert(a.div_unit(UDM(65535)) == 54058); assert(a == NN("134880734929639611284"));
        a.assign("-335456123445434567097887489567346758");
        assert(div_unit(a, UDM(1), b) == 0); assert(b == NN("-335456123445434567097887489567346758"));
        assert(div_unit(a, UDM(3), b) == 2); assert(b == NN("-111818707815144855699295829855782252"));
        assert(div_unit(b, UDM(377), a) == 163); assert(a == NN("-296601346989774153048530052667857"));
        assert(div_unit(a, UDM(1024), a) == 465); assert(a == NN("-289649752919701321336455129558"));
        assert(div_unit(a, UDM(32768), b) == 27094); assert(b == NN("-8839408963613931925550998"));
        assert(div_unit(a, UDM(65535), b) == 54123); assert(b == NN("-4419771922174430782581141"));
        a.assign(437454245000ULL);
        assert(!a.absrem_unit(UDM(50)));
        assert(a.absrem_unit(UDM(12)) == 8);
        assert(!a.absrem_unit(UDM(20)));
        assert(a.absrem_unit(UDM(12345)) == 10010);
        assert(!a.absrem_unit(UDM(5)));
        assert(!a.div_unit(UDM(5))); assert(a == 87490849000ULL);
        assert(!a.div_unit(UDM(20)));
        assert(!a.div_unit(UDM(50)));
    }
}

void test_mod_small()
{
    {
        NN a("94837582435723485723049587239057"), b;
        mod_unit(a, 1234, b); assert(b == 807);
        a.mod_unit(1234); assert(a == 807);
        a.assign("2453562355438");
        mod_unit(a, 1, a); assert(a == 0);
        a.assign("2453562355438");
        a.mod_unit(2); assert(a == 0);
        a.assign("2453562355438");
        mod_unit(a, 3, a); assert(a == 1);
        a.assign("-2453562355438");
        a.mod_unit(3); assert(a == -1);
        a.set_zero();
        a.mod_unit(3); assert(a == 0);
        mod_unit(a, 33399, a); assert(a == 0);
    }{
        NN a("94837582435723485723049587239057");
        a.mod_word(1234); assert(a == 807);
        a.assign("2453562355438");
        a.mod_sword(1); assert(a == 0);
        a.assign("2453562355438");
        a.mod_word(2); assert(a == 0);
        a.assign("2453562355438");
        a.mod_word(3); assert(a == 1);
        a.assign("-2453562355438");
        a.mod_sword(3); assert(a == -1);
        a.set_zero();
        a.mod_word(3); assert(a == 0);
        a.mod_sword(33399); assert(a == 0);

        a.mod_unit(0);
        a.mod(123456);
        mod(a, int(0), a);
        a.mod(long(0));
        a.mod((long long)0);
        a.mod((unsigned int)0);
        a.mod((unsigned long)0);
        a.mod((unsigned long long)0);
    }{
        NN a("335456123445434567097887489567346758"), b;
        mod_unit(b, UDM(1), b); assert(b == 0);
        mod_unit(a, UDM(3), b); assert(b == 2);
        mod_unit(a, UDM(377), b); assert(b == 114);
        mod_unit(a, UDM(1024), b); assert(b == 70);
        mod_unit(a, UDM(32768), b); assert(b == 20550);

        a.assign(4096); a.mod_unit(UDM(32)); assert(a.is_zero());
        a.assign(64); a.mod_unit(UDM(64)); assert(a.is_zero());
        a.assign(65); a.mod_unit(UDM(64)); assert(a.is_one());
    }
}

void test_and_small()
{
    {
        NN a(123), b;
        a.bit_and_unit(123); assert(a == 123);
        a.bit_and_unit(~123); assert(a == 0);
        b.bit_and_unit(123); assert(b == 0);
    }
}

void test_or_small()
{
    {
        NN a(123), b;
        a.bit_or_unit(123); assert(a == 123);
        b.bit_or_unit(123); assert(b == 123);
    }
}

void test_xor_small()
{
    {
        NN a("34095768345980675438723548254968754965843569831234871208957234907");
        a.bit_xor_unit(12345);
        a.bit_xor_word(12345);
    }
    {
        NN a(123), b;
        a.bit_xor_unit(123); assert(a.is_zero());
        b.bit_xor_unit(123); assert(b == 123);
    }{
        NN a(0x12345678), b;
        a.bit_xor_word(0x12345678); assert(a.is_zero());
        b.bit_xor_word(0x12345678); assert(b == 0x12345678);
    }
}

void test_basic_type_conversion()
{
    {
        NN a(INT_MAX), b(LONG_MAX), c(LLONG_MAX);
        assert(a(10) == "2147483647");
        a++; assert(a(10) == "2147483648");
        a++; assert(a(10) == "2147483649");
        assert(c(10) == "9223372036854775807");
        c++; assert(c(10) == "9223372036854775808");
        c++; assert(c(10) == "9223372036854775809");
        if (sizeof(long) == 4) {
            assert(b(10) == "2147483647");
            b++; assert(b(10) == "2147483648");
            b++; assert(b(10) == "2147483649");
        } else {
            assert(b(10) == "9223372036854775807");
            b++; assert(b(10) == "9223372036854775808");
            b++; assert(b(10) == "9223372036854775809");
        }
    }{
        NN a(INT_MIN), b(LONG_MIN), c(LLONG_MIN);
        assert(a(10) == "-2147483648");
        a--; assert(a(10) == "-2147483649");
        a--; assert(a(10) == "-2147483650");
        assert(c(10) == "-9223372036854775808");
        c--; assert(c(10) == "-9223372036854775809");
        c--; assert(c(10) == "-9223372036854775810");
        if (sizeof(long) == 4)
        {
            assert(b(10) == "-2147483648");
            b--; assert(b(10) == "-2147483649");
            b--; assert(b(10) == "-2147483650");
        } else {
            assert(b(10) == "-9223372036854775808");
            b--; assert(b(10) == "-9223372036854775809");
            b--; assert(b(10) == "-9223372036854775810");
        }
    }{
        NN a(INT_MAX), b(INT_MIN), c(LONG_MAX), d(LONG_MIN), e(LLONG_MAX), f(LLONG_MIN);
        assert((a + 3) * 456 == a * 456 + 3 * 456); assert((b + 3) * 456 == b * 456 + 3 * 456);
        assert((c + 3) * 456 == c * 456 + 3 * 456); assert((d + 3) * 456 == d * 456 + 3 * 456);
        assert((e + 3) * 456 == e * 456 + 3 * 456); assert((f + 3) * 456 == f * 456 + 3 * 456);
        assert((a - 8) * 456 == a * 456 - 8 * 456); assert((b - 8) * 456 == b * 456 - 8 * 456);
        assert((c - 8) * 456 == c * 456 - 8 * 456); assert((d - 8) * 456 == d * 456 - 8 * 456);
        assert((e - 8) * 456 == e * 456 - 8 * 456); assert((f - 8) * 456 == f * 456 - 8 * 456);
        assert(a - (a - 1) == 1); assert(b - (b - 1) == 1); assert(c - (c - 1) == 1); assert(d - (d - 1) == 1);
        assert(e - (e - 1) == 1); assert(e - (e - 1) == 1); assert(a - (a - b) == b); assert(b - (b - c) == c);
        assert(c - (c - a) == a); assert(d - (d - f) == f); assert(e - (e - e) == e); assert(e - (e - d) == d);
    }{
        NN a1(123), a2(65536), a3(0xffffff), a4(0xaabbccdd), a5(0xaabbccddee), a6(0xaabbccddeeffULL), a7(0x11aabbccddeeffULL), a8(0x1122aabbccddeeffULL);
        assert(a1.in_range_int()); assert(a2.in_range_int()); assert(a3.in_range_int());
        assert(!a5.in_range_int()); assert(!a5.in_range_uint());
        assert(!a6.in_range_int()); assert(!a6.in_range_uint());
        assert(a6.in_range_ulonglong());
        assert(!a7.in_range_int()); assert(!a7.in_range_uint()); assert(a7.in_range_ulonglong());
        assert(!a8.in_range_int()); assert(!a8.in_range_uint()); assert(a8.in_range_ulonglong());
        assert(a1.to_int() == 123);       assert(a2.to_int() == 65536);       assert(a3.to_int() == 0xffffff);
        assert(a1.to_uint() == 123);      assert(a2.to_uint() == 65536);      assert(a3.to_uint() == 0xffffff);
        assert(a1.to_long() == 123);      assert(a2.to_long() == 65536);      assert(a3.to_long() == 0xffffff);
        assert(a1.to_uint() == 123);      assert(a2.to_uint() == 65536);      assert(a3.to_ulong() == 0xffffff);
        assert(a1.to_longlong() == 123);  assert(a2.to_longlong() == 65536);  assert(a3.to_longlong() == 0xffffff);
        assert(a1.to_ulonglong() == 123); assert(a2.to_ulonglong() == 65536); assert(a3.to_ulonglong() == 0xffffff);
        assert(!a4.in_range_int()); assert(a4.in_range_uint());
        assert(a4.to_long() == 0xaabbccdd); assert(a4.to_ulong() == 0xaabbccdd);
        assert(a4.to_longlong() == 0xaabbccdd); assert(a4.to_ulonglong() == 0xaabbccdd);

        assert(!a5.in_range_int()); assert(!a5.in_range_uint());
        if (sizeof(long) > 4) {
            assert(a5.to_long() == 0xaabbccddee); assert(a5.to_ulong() == 0xaabbccddee);
        } else {
            assert(!a5.in_range_long()); assert(!a5.in_range_ulong());
        }
        assert(a5.to_longlong() == 0xaabbccddee); assert(a5.to_ulonglong() == 0xaabbccddee);

        assert(!a6.in_range_int()); assert(!a6.in_range_uint());
        if (sizeof(long) > 4){
            assert(a6.to_long() == 0xaabbccddeeffULL); assert(a6.to_ulong() == 0xaabbccddeeffULL);
        } else {
            assert(!a6.in_range_long()); assert(!a6.in_range_ulong());
        }
        assert(a6.to_longlong() == 0xaabbccddeeffULL); assert(a6.to_ulonglong() == 0xaabbccddeeffULL);

        assert(!a7.in_range_int()); assert(!a7.in_range_uint());
        if (sizeof(long) > 4)
        {
            assert(a7.to_long() == 0x11aabbccddeeffULL); assert(a7.to_ulong() == 0x11aabbccddeeffULL);
        } else {
            assert(!a7.in_range_long()); assert(!a7.in_range_ulong());
        }
        assert(a7.to_longlong() == 0x11aabbccddeeffULL); assert(a7.to_ulonglong() == 0x11aabbccddeeffULL);

        assert(!a8.in_range_int()); assert(!a8.in_range_uint());
        if (sizeof(long) > 4) {
            assert(a8.to_long() == 0x1122aabbccddeeffULL); assert(a8.to_ulong() == 0x1122aabbccddeeffULL);
        } else {
            assert(!a8.in_range_long()); assert(!a8.in_range_ulong());
        }
        assert(a8.to_longlong() == 0x1122aabbccddeeffULL); assert(a8.to_ulonglong() == 0x1122aabbccddeeffULL);
    }{
        NN a, b(INT_MAX), c(INT_MIN), d(LONG_MAX), e(LONG_MIN), f(LLONG_MAX), g(LLONG_MIN);
        assert(a.in_range_int()); assert(a.in_range_uint()); assert(a.in_range_long()); assert(a.in_range_ulong()); assert(a.in_range_longlong()); assert(a.in_range_ulonglong());
        assert(b.in_range_int()); assert(b.in_range_uint()); assert(b.in_range_long()); assert(b.in_range_ulong()); assert(b.in_range_longlong()); assert(b.in_range_ulonglong());
        assert(c.in_range_int()); assert(!c.in_range_uint()); assert(c.in_range_long()); assert(c.in_range_longlong());
        assert(d.in_range_long()); assert(d.in_range_ulong()); assert(d.in_range_longlong()); assert(d.in_range_ulonglong());
        assert(e.in_range_long()); assert(e.in_range_longlong());
        assert(f.in_range_longlong()); assert(f.in_range_ulonglong());
        assert(g.in_range_longlong()); assert(!g.in_range_ulonglong());
        assert(b.to_int() == INT_MAX); assert(c.to_int() == INT_MIN);
        assert(d.to_long() == LONG_MAX); assert(e.to_long() == LONG_MIN);
        assert(f.to_longlong() == LLONG_MAX); assert(g.to_longlong() == LLONG_MIN);
        b++; assert(!b.in_range_int()); b++; assert(!b.in_range_int()); b--; assert(!b.in_range_int()); b--; assert(b.in_range_int());
        c++; assert(c.in_range_int()); c++; assert(c.in_range_int()); c--; assert(c.in_range_int()); c--; assert(c.in_range_int()); c--; assert(!c.in_range_int());
        d++; assert(!d.in_range_long()); d++; assert(!d.in_range_long()); d--; assert(!d.in_range_long()); d--; assert(d.in_range_long());
        e++; assert(e.in_range_long()); e++; assert(e.in_range_long()); e--; assert(e.in_range_long()); e--; assert(e.in_range_long()); e--; assert(!e.in_range_long());
        f++; assert(!f.in_range_longlong()); f++; assert(!f.in_range_longlong()); f--; assert(!f.in_range_longlong()); f--; assert(f.in_range_longlong());
        g++; assert(g.in_range_longlong()); g++; assert(g.in_range_longlong()); g--; assert(g.in_range_longlong()); g--; assert(g.in_range_longlong()); g--; assert(!g.in_range_longlong());
    }{
        NN a, b(UINT_MAX), c(ULONG_MAX), d(ULLONG_MAX);
        assert(a.in_range_uint()); assert(a.in_range_ulong());assert(a.in_range_ulonglong());
        assert(!b.in_range_int()); assert(b.in_range_uint()); assert(b.in_range_ulong()); assert(b.in_range_ulonglong());
        assert(!c.in_range_int()); assert(c.in_range_ulong()); assert(c.in_range_ulonglong());
        assert(b.to_uint() == UINT_MAX); assert(c.to_ulong() == ULONG_MAX); assert(d.to_ulonglong() == ULLONG_MAX);
        b++; assert(!b.in_range_int()); assert(!b.in_range_uint());
        c++; assert(!c.in_range_long()); assert(!c.in_range_ulong());
        d++; assert(!d.in_range_longlong()); assert(!d.in_range_ulonglong());
    }{
        NN a(CHAR_MAX), b(CHAR_MIN), c(SHRT_MAX), d(SHRT_MIN);
        assert(a.in_range_char()); assert(a.in_range_uchar()); assert(a.in_range_ushort()); assert(a.in_range_uint());
        assert(b.in_range_char()); assert(!b.in_range_uchar()); assert(b.in_range_short()); assert(!b.in_range_uint());
        assert(!c.in_range_char()); assert(!c.in_range_uchar()); assert(c.in_range_short()); assert(c.in_range_ushort());
        assert(!d.in_range_char()); assert(!d.in_range_uchar()); assert(d.in_range_short()); assert(!d.in_range_ushort()); assert(d.in_range_int());
        assert(a.to_char() == CHAR_MAX); assert(b.to_char() == CHAR_MIN); assert(c.to_short() == SHRT_MAX); assert(d.to_short() == SHRT_MIN);
        a--; assert(a.in_range_char()); a--; assert(a.in_range_char()); a++; assert(a.in_range_char()); a++; assert(a.in_range_char());
        b--; assert(!b.in_range_char()); b--; assert(!b.in_range_char()); b++; assert(!b.in_range_char()); b++; assert(b.in_range_char());
        c--; assert(c.in_range_short()); c--; assert(c.in_range_short()); c++; assert(c.in_range_short()); c++; assert(c.in_range_short());
        d--; assert(!d.in_range_short()); d--; assert(!d.in_range_short()); d++; assert(!d.in_range_short()); d++; assert(d.in_range_short());
    }
}

void test_prime()
{
    {
        assert(!prime_test_roughly(0));
        assert(!prime_test_roughly(1));

        assert(prime_test_roughly(2));
        assert(prime_test_roughly(3));
        assert(!prime_test_roughly(4));
        assert(prime_test_roughly(5));

        assert(prime_test_roughly(233));
        assert(prime_test_roughly(239));

        assert(!prime_test_roughly(561)); // Carmichael
        assert(!prime_test_roughly(1105));
        assert(!prime_test_roughly(1729));
        assert(prime_test_roughly(19937)); // prime
        assert(prime_test_roughly(54503));
        assert(prime_test_roughly(54517));
        assert(prime_test_roughly(54521));
    }{
        NN next;
        prime_next_roughly(-8, next); assert(next == 2);
        prime_next_roughly(0, next); assert(next == 2);
        prime_next_roughly(1, next); assert(next == 2);
        prime_next_roughly(2, next); assert(next == 3);
        prime_next_roughly(3, next); assert(next == 5);
        prime_next_roughly(233, next); assert(next == 239);
        prime_next_roughly(239, next); assert(next == 241);
        prime_next_roughly(74093, next); assert(next == 74099);
    }{
        NN prev;
        prime_prev_roughly(3, prev); assert(prev == 2);
        prime_prev_roughly(4, prev); assert(prev == 3);
        prime_prev_roughly(239, prev); assert(prev == 233);
        prime_prev_roughly(983, prev); assert(prev == 977);
        prime_prev_roughly(74093, prev); assert(prev == 74077);
    }
}

bool chance(int n)
{
    return rand() % n == 1;
}

void random(number_t& a)
{
    if (a.len)
    {
        int i = 0;
        while (i < a.len)
        {
            a.dat[i++] = (unit_t)rand();
        }
        if (a.dat[a.len - 1] == 0)
        {
            a.dat[a.len - 1] = 1;
        }
    }
}

void create_big(NN& x, int size)
{
    x.set_zero();
    x.bit_set_one(size * sizeof(unit_t) * 8);
    random(x);
}
