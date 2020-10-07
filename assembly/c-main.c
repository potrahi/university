#include <stdio.h>

char g_pozdrav[ 32 ] = "_Ahoj!",
	 g_pole_bajty[ 4 ] = { 0xA1, 0xB2, 0xC3, 0xD4 },
	 g_vypis[ 11 ] = "XXXXXXXXXX",
	 g_pocet = 0,
	 g_dnes[] = "Dnes je 13. 13. 2303 12:62:75",
	 g_cislo_binarne[ 33 ] = "",
	 l_cisla_bitu[] = { 56, 48, 12, 11, 17 },
	 l_text[] = "Obed o 5 chodech se podava ve 12 hodin.",
	 l_pismena[ 1024 ];

int g_val0 = -10,
	g_val1 = 10,
	g_4bajty = 0,
	g_lsb = 0,
	g_msb = 0,
	g_cislo = 123456789,
	pocet = 0,
	t_LEN = sizeof(l_cisla_bitu),
	g_int_pole[ 256 ],
	login = 38,
	count_4x0 = 0,
	count_4x1 = 0,
	l_pole[] = { 8000, -777, 33, 444, 555, -666, 0xFFF, -888 },
	l_pozice_nejnej[ 2 ],
	prenes_pismena( char *t_pismena, char *t_text, char zastupny_znak ),
	overeni( char *retezec ),
	nasobky_prvocisel_z( int *t_pole, int t_start, int t_koef ),
	nasobky_prvocisel_s( int *t_pole, int t_start, int t_koef );

long g_pole_long1[ 6 ] = { 0, 0, 0, 0, 0, 0 },
	 g_pole_long2[ 6 ] = { 0, 0, 0, 0, 0, 0 },
	 g_long = 0xFEDCBA9876543210,
	 g_heslo = 0x336E6164417A656E,
	 //g_long_n = 0x2121212141414141,
	 g_long_n = 0x0000000000000009,
	 bitova_maska( char *l_cisla_bitu, int t_LEN );

void presun(),
	 presun_bz(),
	 posun(),
	 sloz(),
	 rozloz(),
	 preved(),
	 spocitej(),
	 kolik_cislic(),
	 do_dvojkove(),
	 count(),
	 kde_je_minmax( int *t_cela_cisla, int t_N, int *t_pozice_nejv_nejm ),
	 povrch_trubky( int t_R, int t_delka, int *t_vysledek );


void print_array_long(long *array, int arr_size)
{
	for(int i = 0; i < arr_size; i++)
	{
		printf("%ld ", array[i]);
	}
	printf("\n");
}

void print_array_char(char *array, int arr_size)
{
	for(int i = 0; i < arr_size; i++)
	{
		printf("%c ", array[i]);
	}
	printf("\n");
}

void print_64_in_bin(long num)
{
	int index = 63;
	long mask = 1;
    char num_bin[65] = "";
    for (int i = 0; i < 64; i++)
    {
        if (num & mask)
        {
            num_bin[index] = '1';
        }
        else
        {
            num_bin[index] = '0';
        }
        index--;
        mask = mask << 1;
    }
    printf("%s\n", num_bin);
}
int prvocisla(long *t_pole, int t_N, long *t_prvocisla),
	hledej_rozptyl(int *t_pole, int t_N );


void najdi_minmax(int *pole, int delka, int volba);
void kolik_jednicek(int *pole, int *polejednicek, int delka);

int int2bit(char *string, long *num);

void zdrcni(char *nesmysl);

int main()
{
	print_array_long(g_pole_long1, 6);
	presun();
	print_array_long(g_pole_long1, 6);

	print_array_long(g_pole_long2, 6);
	presun_bz();
	print_array_long(g_pole_long2, 6);

	print_array_char(g_pozdrav, 32);
	posun();
	print_array_char(g_pozdrav, 32);

	sloz();
	printf( "%08X\n", g_4bajty );

	rozloz();
	printf( "long %016lX, msb %08X, lsb %08X\n", g_long, g_msb, g_lsb );

	preved();
	printf( "heslo: '%s'\n", g_vypis );

	spocitej();
	printf( "g_long_n = %d\n", g_pocet );

	kolik_cislic();
	printf( "pocet cislic = %d\n", pocet );

	do_dvojkove();
	printf("num %d in binary = %s\n", g_cislo, g_cislo_binarne);

	for(int i = 0; i < 256; i++)
	{
		g_int_pole[i] = i * (login | 1);
	}
	count();
	printf("count_4x0 = %d\ncount_4x1 = %d\n", count_4x0, count_4x1);
	//count_4x0 = 0, count_4x1 = 0;
	count();
	printf("count_4x0 = %d\ncount_4x1 = %d\n", count_4x0, count_4x1);
	long result = bitova_maska(l_cisla_bitu, t_LEN);
	printf("bit elone_mask in HEX = %016lX\nAnd in DEC = %ld\n", result, result);
	print_64_in_bin(result);

	kde_je_minmax( l_pole, sizeof(l_pole)/4, l_pozice_nejnej );
	printf("nejmensi = %d\nnejvetsi = %d\n", l_pozice_nejnej[1], l_pozice_nejnej[0]);

	int res = prenes_pismena( l_pismena, l_text, '^' );
	printf("copied chars = %d\npismena = %s\n", res, l_pismena);

    char *string_array[] = {"..67", "-33_245", ".", "+44..", "_4.5.", "123.999", "1_2_3", "_-3.", ".5", "-.99"};
    for (int i = 0; i < 10; i++)
    {
        int odpoved = 0;
        odpoved = overeni(string_array[i]);
        printf("%s \t=> %d \n", string_array[i], odpoved);
    }

    long l_pole[9] = {111, 20, 21, 133, 47, 155, 67, 171, 83};
    long l_prvocisla[] = {};
    int l_pocet = prvocisla(l_pole, 9, l_prvocisla);
    printf("%d \n", l_pocet);

    int pole[] = {-10, 55, 2, 0};
    int res_cislo = hledej_rozptyl(pole, 4);
    printf("%d \n", res_cislo);

    int l_vysledek[2];
    povrch_trubky(10, 100, l_vysledek);
    printf("Povrch trubky je %d.%03d\n", l_vysledek[0], l_vysledek[1]);

    int t_start = 1;
    int t_koef = 41;
    int l_cisla[1024];
    int l_count = 0;
    if(t_koef < 0)
    {
    	l_count = nasobky_prvocisel_s( l_cisla, t_start, t_koef );
    }
    else
    {
    	l_count = nasobky_prvocisel_z( l_cisla, t_start, t_koef );
    }
    for(int i = 0; i < l_count; i++)
    {
    	printf("%d ", l_cisla[i]);
    }
    printf("\n");
    printf("%d \n", l_count);


    int polepole[] = {-6, 2, 0, 5, 0xAA};
    int polejednicek[] = {};

    kolik_jednicek(polepole, polejednicek, 5);
    printf("pocet jednicek: ");
    for(int i = 0; i < 5; i++)
	{
		printf("%d ", polejednicek[i]);
	}
    printf("\n");

    najdi_minmax(polepole, 3, 1);
    printf("minMAX: ");
    for(int i = 0; i < 3; i++)
    {
    	printf("%d ", polepole[i]);
    }
    printf("\n");


    char string[] = "-1564";
    long resalt_value = 0;

    int vysledek = 0;
    vysledek = int2bit(string, &resalt_value);
    printf("resalt_value -> %ld\nvysledek -> %d\n", resalt_value, vysledek);
    vysledek = int2bit("++15", &resalt_value);
    printf("resalt_value -> %ld\nvysledek -> %d\n", resalt_value, vysledek);
    vysledek = int2bit("34567890", &resalt_value);
    printf("resalt_value -> %ld\nvysledek -> %d\n", resalt_value, vysledek);
    vysledek = int2bit("333A", &resalt_value);
    printf("resalt_value -> %ld\nvysledek -> %d\n", resalt_value, vysledek);

    char nesmysl[] = "aaabbcddde";
    zdrcni(nesmysl);
    printf("%s \n", nesmysl);
}
