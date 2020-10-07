#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

using namespace std;

// class RDSReader for reading captured RDS data from FM radio
class RDSReader
{
public:
    // constructor without argumets
    RDSReader()
    {
        m_data_file = nullptr;
        m_file_name[0] = 0;
    }

    // destructor
    ~RDSReader()
    {
        close_file();
    }

    // Open file with captured data, argument is file name
    int open_file(const char *t_file_name)
    {
        if (!t_file_name || !t_file_name[0])
        {
            fprintf(stderr, "No file name specified!\n");
            return -1;
        }
        strcpy(m_file_name, t_file_name);
        return reopen_file();
    }

    // Reopen file
    int reopen_file()
    {
        close_file();

        m_data_file = fopen(m_file_name, "r");
        if (!m_data_file)
        {
            fprintf(stderr, "Unable to open data file '%s'!\n", m_file_name);
            return -1;
        }
        return 0;
    }

    // Close file
    int close_file()
    {
        if (m_data_file)
        {
            fclose(m_data_file);
            m_data_file = nullptr;
            return 0;
        }
        return -1;
    }

    // Method get_rds_status returns 13 bytes read by FM radio command
    // 0x24 - FM_RDS_STATUS.
    // Data is returned in the same order as it is specified in datasheet.
    // Argument is void to be possible use any data type.
    int get_rds_status(void *t_data)
    {
        if (!t_data || !m_data_file)
            return -1;

        char *l_data = (char *)t_data;
        char l_file_line[1024];
        if (!fgets(l_file_line, sizeof(l_file_line), m_data_file))
            return 0;

        int l_skip_chars = 8;
        while (1)
        {
            int l_tmp, l_read;
            if (sscanf(l_file_line + l_skip_chars, "%X%n", &l_tmp, &l_read) < 1)
                break;
            *(l_data++) = l_tmp;
            l_skip_chars += l_read;
        }
        return l_data - (char *)t_data;
    }

    int skupina(uint8_t *moje_data) // GROUP
    {
        int group = 0;
        switch (moje_data[6] >> 3)
        {
        case 0:
            group = 10;
            break;
        case 4:
            group = 12;
            break;
        case 8:
            group = 14;
            break;
        default:
            break;
        }
        return group;
    }

    bool platna_data(uint8_t *moje_data) // VALID DATA CHECK
    {
        if (!(moje_data[1] & 1))
        {
            return false;
        }
        else if (moje_data[1] & 2)
        {
            return false;
        }
        else if (!(moje_data[1] & 4) && !(moje_data[2] & 1))
        {
            return false;
        }
        else if (moje_data[2] & 4)
        {
            return false;
        }
        else if ((moje_data[1] & 4) && (moje_data[2] & 1) && !(moje_data[1] >> 4 & 3))
        {
            return false;
        }
        else if (moje_data[3] == 0)
        {
            return false;
        }
        else if ((moje_data[12] & 3) || (moje_data[12] >> 2 & 3) || (moje_data[12] >> 4 & 3) || (moje_data[12] >> 6 & 3))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    int pi_code(uint8_t *moje_data) // PIcode
    {
        int picode = 0;
        picode = (moje_data[4] << 8) | moje_data[5];
        return picode;
    }

    void zpracuj_0A(uint8_t *l_moje_data, uint8_t *l_nazev_stanice) // NAME STATION
    {
        switch (l_moje_data[7] & 0b00000011)
        {
        case 0: // d3
            l_nazev_stanice[0] = l_moje_data[10];
            l_nazev_stanice[1] = l_moje_data[11];
            break;
        case 1: // d2
            l_nazev_stanice[2] = l_moje_data[10];
            l_nazev_stanice[3] = l_moje_data[11];
            break;
        case 2: // d1
            l_nazev_stanice[4] = l_moje_data[10];
            l_nazev_stanice[5] = l_moje_data[11];
            break;
        case 3: // d0
            l_nazev_stanice[6] = l_moje_data[10];
            l_nazev_stanice[7] = l_moje_data[11];
            break;
        default:
            break;
        }
    }

    void zpracuj_2A(uint8_t *l_moje_data, uint8_t *l_radiotext) // RADIOTEXT
    {
        int mask = 0b00000001;
        if (l_moje_data[7] & mask)
        {
            // ???1
            mask = mask << 1;
            if (l_moje_data[7] & mask)
            {
                // ??11
                mask = mask << 1;
                if (l_moje_data[7] & mask)
                {
                    // ?111
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1111
                        l_radiotext[60] = l_moje_data[8];
                        l_radiotext[61] = l_moje_data[9];
                        l_radiotext[62] = l_moje_data[10];
                        l_radiotext[63] = l_moje_data[11];
                    }
                    else
                    {
                        // 0111
                        l_radiotext[28] = l_moje_data[8];
                        l_radiotext[29] = l_moje_data[9];
                        l_radiotext[30] = l_moje_data[10];
                        l_radiotext[31] = l_moje_data[11];
                    }
                }
                else
                {
                    // ?011
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1011
                        l_radiotext[44] = l_moje_data[8];
                        l_radiotext[45] = l_moje_data[9];
                        l_radiotext[46] = l_moje_data[10];
                        l_radiotext[47] = l_moje_data[11];
                    }
                    else
                    {
                        // 0011
                        l_radiotext[12] = l_moje_data[8];
                        l_radiotext[13] = l_moje_data[9];
                        l_radiotext[14] = l_moje_data[10];
                        l_radiotext[15] = l_moje_data[11];
                    }
                }
            }
            else
            {
                // ??01
                mask = mask << 1;
                if (l_moje_data[7] & mask)
                {
                    // ?101
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1101
                        l_radiotext[52] = l_moje_data[8];
                        l_radiotext[53] = l_moje_data[9];
                        l_radiotext[54] = l_moje_data[10];
                        l_radiotext[55] = l_moje_data[11];
                    }
                    else
                    {
                        // 0101
                        l_radiotext[20] = l_moje_data[8];
                        l_radiotext[21] = l_moje_data[9];
                        l_radiotext[22] = l_moje_data[10];
                        l_radiotext[23] = l_moje_data[11];
                    }
                }
                else
                {
                    // ?001
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1001
                        l_radiotext[36] = l_moje_data[8];
                        l_radiotext[37] = l_moje_data[9];
                        l_radiotext[38] = l_moje_data[10];
                        l_radiotext[39] = l_moje_data[11];
                    }
                    else
                    {
                        // 0001
                        l_radiotext[4] = l_moje_data[8];
                        l_radiotext[5] = l_moje_data[9];
                        l_radiotext[6] = l_moje_data[10];
                        l_radiotext[7] = l_moje_data[11];
                    }
                }
            }
        }
        else
        {
            // ???0
            mask = mask << 1;
            if (l_moje_data[7] & mask)
            {
                // ??10
                mask = mask << 1;
                if (l_moje_data[7] & mask)
                {
                    // ?110
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1110
                        l_radiotext[56] = l_moje_data[8];
                        l_radiotext[57] = l_moje_data[9];
                        l_radiotext[58] = l_moje_data[10];
                        l_radiotext[59] = l_moje_data[11];
                    }
                    else
                    {
                        // 0110
                        l_radiotext[24] = l_moje_data[8];
                        l_radiotext[25] = l_moje_data[9];
                        l_radiotext[26] = l_moje_data[10];
                        l_radiotext[27] = l_moje_data[11];
                    }
                }
                else
                {
                    // ?010
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1010
                        l_radiotext[40] = l_moje_data[8];
                        l_radiotext[41] = l_moje_data[9];
                        l_radiotext[42] = l_moje_data[10];
                        l_radiotext[43] = l_moje_data[11];
                    }
                    else
                    {
                        // 0010
                        l_radiotext[8] = l_moje_data[8];
                        l_radiotext[9] = l_moje_data[9];
                        l_radiotext[10] = l_moje_data[10];
                        l_radiotext[11] = l_moje_data[11];
                    }
                }
            }
            else
            {
                // ??00
                mask = mask << 1;
                if (l_moje_data[7] & mask)
                {
                    // ?100
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1100
                        l_radiotext[48] = l_moje_data[8];
                        l_radiotext[49] = l_moje_data[9];
                        l_radiotext[50] = l_moje_data[10];
                        l_radiotext[51] = l_moje_data[11];
                    }
                    else
                    {
                        // 0100
                        l_radiotext[16] = l_moje_data[8];
                        l_radiotext[17] = l_moje_data[9];
                        l_radiotext[18] = l_moje_data[10];
                        l_radiotext[19] = l_moje_data[11];
                    }
                }
                else
                {
                    // ?000
                    mask = mask << 1;
                    if (l_moje_data[7] & mask)
                    {
                        // 1000
                        l_radiotext[32] = l_moje_data[8];
                        l_radiotext[33] = l_moje_data[9];
                        l_radiotext[34] = l_moje_data[10];
                        l_radiotext[35] = l_moje_data[11];
                    }
                    else
                    {
                        // 0000
                        l_radiotext[0] = l_moje_data[8];
                        l_radiotext[1] = l_moje_data[9];
                        l_radiotext[2] = l_moje_data[10];
                        l_radiotext[3] = l_moje_data[11];
                    }
                }
            }
        }
    }

    int zpracuj_4A(uint8_t *l_moje_data, char *calendar) // TIME
    {
        int l_local_time_offset = 0;
        // LOCAL TIME OFFSET
        int znak = 0;
        if ((l_moje_data[11] >> 5) & 0b00000001)
        {
            znak = 1;
        }
        if (znak == 1)
        {
            l_local_time_offset = l_moje_data[11] & 0b00011111;
            l_local_time_offset *= -1;
        }
        else
        {
            l_local_time_offset = l_moje_data[11] & 0b00011111;
        }
        // MINUTE
        int l_minute = 0;
        l_minute = ((l_moje_data[11] >> 6) | ((l_moje_data[10] & 0b00001111) << 2));
        // HOUR
        int l_hour = 0;
        l_hour = ((l_moje_data[10] >> 4) | ((l_moje_data[9] & 0b00000001) << 4));

        int time = 0;
        time = (l_hour * 60 + l_minute + l_local_time_offset * 30) * 60;

        int MJD = 0;
        MJD = ((((l_moje_data[7] & 0b00000011) << 15) | (l_moje_data[8] << 7)) | (l_moje_data[9] >> 1));

        int _Y = 0;
        _Y = int((MJD - 15078.2) / 365.25);

        int _M = 0;
        _M = int((MJD - 14956.1 - int(_Y * 365.25)) / 30.6001);

        int D = 0;
        D = MJD - 14956 - int(_Y * 365.25) - int(_M * 30.6001);

        int K = 0;
        if (_M == 14 || _M == 15)
        {
            K = 1;
        }
        else
        {
            K = 0;
        }
        int Y = 0;
        Y = _Y + K + 1900;

        int M = 0;
        M = _M - 1 - K * 12;

        int WD = 0;
        WD = (((MJD + 2) % 7));

        int W = 0;
        W = int((MJD / 7) - 2144.64);

        int WY = 0;
        WY = int((W * 28 / 1461) - 0.0079);

        int WN = 0;
        WN = W - int((WY * 1461 / 28) + 0.41);

        char *den_nedeli[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
        sprintf(calendar, "%s of %d.%d.%d", den_nedeli[WD], D, M, Y);
        return time;
    }

protected:
    FILE *m_data_file;
    char m_file_name[1024];
};

#define FM_RDS_STATUS_ANS_LEN 13
#define NEPLATI 0
#define SKUPINA_0A 10
#define SKUPINA_2A 12
#define SKUPINA_4A 14

int main(int t_argn, char **t_argc)
{
    if (t_argn < 2)
    {
        printf("Usage: rds.dat\n");
        exit(1);
    }

    ofstream result, all_results;
    result.open("result.txt");
    all_results.open("all_results.txt");

    RDSReader rds;
    rds.open_file(t_argc[1]);

    uint8_t l_moje_data[FM_RDS_STATUS_ANS_LEN];
    uint8_t l_nazev_stanice[9] = {};
    uint8_t l_radiotext[65] = {};
    char l_cas[6] = {};
    int l_old_picode = 0;

    int time = 0;
    int hour = 0;
    int minute = 0;
    char calendar[24] = {};

    int zacatek = 0;
    int konec = 0;
    int trvani = 0;
    char time_zacatek[6] = {};
    char jak_dlouho[6] = {};

    int zacatek_hodiny = 0;
    int zacatek_minuty = 0;

    int flag = 0;
    int count = 1;

    while (rds.get_rds_status(l_moje_data) == FM_RDS_STATUS_ANS_LEN)
    {
        if (rds.platna_data(l_moje_data) == NEPLATI)
            continue;

        int l_picode = rds.pi_code(l_moje_data);

        if (rds.skupina(l_moje_data) == SKUPINA_0A)
            rds.zpracuj_0A(l_moje_data, l_nazev_stanice);
        if (rds.skupina(l_moje_data) == SKUPINA_2A)
        {
            rds.zpracuj_2A(l_moje_data, l_radiotext);
            int i = 0;
            while (l_radiotext[i] != '\0')
            {
                if (l_radiotext[i] < 32)
                {
                    l_radiotext[i] = ' ';
                }
                i++;
            }
        }
        if (rds.skupina(l_moje_data) == SKUPINA_4A)
        {
            time = rds.zpracuj_4A(l_moje_data, calendar);
            hour = time / 3600;
            int next_time = time % 3600;
            minute = next_time / 60;
            if (minute < 10)
            {
                sprintf(l_cas, "%d:0%d", hour, minute);
            }
            else
                sprintf(l_cas, "%d:%d", hour, minute);
        }
        if ((rds.skupina(l_moje_data) == SKUPINA_0A) && ((l_moje_data[7] >> 4) & 1) && (flag == 0))
        {
            flag = 1;
            zacatek = time;
            hour = zacatek / 3600;
            int next_time = zacatek % 3600;
            minute = next_time / 60;
            zacatek_hodiny = hour;
            zacatek_minuty = minute;
            if (minute < 10)
            {
                sprintf(time_zacatek, "%d:0%d", zacatek_hodiny, zacatek_minuty);
            }
            else
                sprintf(time_zacatek, "%d:%d", zacatek_hodiny, zacatek_minuty);
        }
        if ((rds.skupina(l_moje_data) == SKUPINA_0A) && !((l_moje_data[7] >> 4) & 1) && (flag == 1))
        {
            flag = 0;
            konec = time;
            trvani = konec - zacatek;
            hour = trvani / 3600;
            int next_time = trvani % 3600;
            minute = next_time / 60;
            if (minute < 10)
            {
                sprintf(jak_dlouho, "%d:0%d", hour, minute);
            }
            else
                sprintf(jak_dlouho, "%d:%d", hour, minute);
            result << "TA on " << l_picode << " '" << l_nazev_stanice << "' in " << time_zacatek << " and end in " << l_cas << ". " << jak_dlouho << " long. Radiotext '" << l_radiotext << "'\n";
        }
        if (rds.skupina(l_moje_data) == SKUPINA_0A && (l_moje_data[7] >> 4) & 1)
            all_results << "Cas: " << l_cas << "." << calendar << ". Vysila stanice: " << l_picode << ". Nazev: '" << l_nazev_stanice << "'. Radiotext: '" << l_radiotext << "' TA 1\n";
        else
            all_results << "Cas: " << l_cas << "." << calendar << ". Vysila stanice: " << l_picode << ". Nazev: '" << l_nazev_stanice << "'. Radiotext: '" << l_radiotext << "' TA 0\n";
        int i = 0;
        if (l_picode != l_old_picode)
        {
            l_old_picode = l_picode;
            while (l_radiotext[i] != '\0')
            {
                l_radiotext[i] = '\0';
                i++;
            }
        }
    }

    result.close();
    all_results.close();
    printf("Zkoncil cist.\n");
}
