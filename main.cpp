#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

using namespace std;

bool v = false, vv = false;  // Verbose output flag

/////////////////////////////////////////////////
// ===== SHA-256 CONSTANTS AND FUNCTIONS ===== //
/////////////////////////////////////////////////

//  H0 - H7 registers
uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

// K constants table (pre-calculated)
uint32_t K[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

uint32_t rot_right(uint32_t a, int n) {
    n = n % 32;  // Safeguard against wiping rotated input
    return(((a << (32 - n)) | (a >> n)) & 0xFFFFFFFF);
}

uint32_t Ch(uint32_t e, uint32_t f, uint32_t g) { return (e & f) ^ (~e & g); }

uint32_t Maj(uint32_t a, uint32_t b, uint32_t c) { return (a & b) ^ (a & c) ^ (b & c); }

uint32_t S0(uint32_t a) { return rot_right(a, 2) ^ rot_right(a, 13) ^ rot_right(a, 22); }

uint32_t S1(uint32_t a) { return rot_right(a, 6) ^ rot_right(a, 11) ^ rot_right(a, 25); }

void pad_message(vector<unsigned char> &buffer){

    if (v) cout << "\t[v] Padding started..." << endl;
    uint64_t original_length = buffer.size() * 8;

    buffer.push_back((unsigned char)0x80);  // Adding set byte after message

    while (buffer.size()%64 != 56) {  // Appending zeros until 56 bytes reached in last block
        buffer.push_back((char)0x00);
    }
    
    char *length_bytes {reinterpret_cast<char*>(&original_length)};
    for (int i = 7; i >= 0; i--) buffer.push_back(length_bytes[i]);  // Filling last 8 bytes with input length
}



/// @brief 
/// @return 
int main(int argc, char *argv[]){

    ///////////////////////////////
    // ===== PARSING INPUT ===== //
    ///////////////////////////////
    
    string in_path {}, out_path {}, compare{};
    bool c = false, h = false;

    if (argc > 1){
        int i{};
        string arg;
        for (i = 0; i < argc; i++){
            arg = argv[i];
            if (arg == "-h"){  // Print help
                cout << "SHA-256 tool by Tombuster - help:\n";
                cout << "\t-i <input_path>\t\tInput file path\n";
                cout << "\t-o <output_path>\tOutput file path (if you'd like to write the output to a file)\n";
                cout << "\t-c <hash>\t\tCheck calculated hash against the one provided after -c\n";
                cout << "\t-v\t\t\tVerbose\n";
                cout << "\t-vv\t\t\tVery verbose\n";
                cout << "\t-h\t\t\tDisplay this help message\n";
                cout << "If no input file path is provided, you will be prompted to add one through the command line.\n";
                cout << "\nThanks for downloading! (^-^d)\n\n";
                return 0;
            }
            else if (arg == "-i" && i + 1 < argc){
                in_path = argv[++i];
            }
            else if (arg == "-o" && i + 1 < argc){
                out_path = argv[++i];
            }
            else if (arg == "-c" && i + 1 < argc){
                c = true;
                compare = argv[++i];
            }
            else if (arg == "-v"){
                v = true;
            }
            else if (arg == "-vv"){
                v = true;
                vv = true;
            }
        }
    }

    if (vv){
        cout << "\t[vv] Received flags: ";
        if (!in_path.empty()) cout << "i, ";
        if (!out_path.empty()) cout << "o, ";
        if (c) cout << "c, ";
        cout << "vv." << endl;
    }

    if (in_path.empty()){
        cout << "Please provide full path to INPUT file:\n> ";
        getline(cin, in_path);
    }


    /////////////////////////////////
    // ===== PREPARING INPUT ===== //
    /////////////////////////////////
    
    int i;

    ifstream infile(in_path, ios_base::binary);  // Read file as bits
    if (vv) cout << "\t[vv] File " << in_path << " opened..." << endl;

    vector<unsigned char> buffer {istreambuf_iterator<char>(infile), istreambuf_iterator<char>()};
    if (v) cout << "\t[v] File " << in_path << " buffered successfully...\n\t    Read " << buffer.size() << " bytes..." << endl;

    infile.close();
    if (vv) cout << "\t[vv] File " << in_path << " closed...\n" << endl;

    pad_message(buffer);
    if (v) cout << "\t[v] Padding successful...\n\t    Length is " << buffer.size() << " bytes..." << endl;


    //////////////////////////////////
    // ===== CALCULATING HASH ===== //
    //////////////////////////////////

    if (v) cout << "\t[v] Beginning hash calculations..." << endl;
    int j, w_index;
    for (i = 0; i < buffer.size(); i += 64){  // Extracting 512-bit long blocks

        if (vv) cout << "\t[vv] Processing block #" << i/64 << "..." << endl;
        uint32_t w[64] {}, tmp1, tmp2, s1, ch, t1, s0, maj, t2, h[8] {};

        for (j = 0; j < 64; j += 4){
            w[j/4] = (buffer[j] << 24) + (buffer[j+1] << 16) + (buffer[j+2] << 8) + buffer[j+3];
        }
        if (vv) cout << "\t[vv] Parsed 16 32-bit long words...\n\t     Calculating 48 more words..." << endl;

        for (j = 16; j < 64; j++){
            tmp1 = rot_right(w[j - 15], 7) ^ rot_right(w[j - 15], 18) ^ (w[j - 15] >> 3);
            tmp2 = rot_right(w[j - 2], 17) ^ rot_right(w[j - 2], 19) ^ (w[j - 2] >> 10);
            w[j] = ((w[j - 16] + tmp1 + w[j - 7] + tmp2) & 0xFFFFFFFF);
        }
        if (vv) cout << "\t[vv] Successfully calculated additional words..." << endl;

        copy(begin(H), end(H), begin(h));
        if (vv) cout << "\t[vv] Copied H register...\n\t     Beginning calculations..." << endl;

        for (j = 0; j < 64; j++){
            s1 = S1(h[4]);
            ch = Ch(h[4], h[5], h[6]);
            t1 = ((h[7] + s1 + ch + K[j] + w[j]) & 0xFFFFFFFF);
            s0 = S0(h[0]);
            maj = Maj(h[0], h[1], h[2]);
            t2 = (s0 + maj) & 0xFFFFFFFF;

            h[7] = h[6];
            h[6] = h[5];
            h[5] = h[4];
            h[4] = (h[3] + t1) & 0xFFFFFFFF;
            h[3] = h[2];
            h[2] = h[1];
            h[1] = h[0];
            h[0] = (t1 + t2) & 0xFFFFFFFF;
        }
        if (vv) cout << "\t[vv] Calculations successful...\n\t     Updating H register..." << endl;

        H[0] = (H[0] + h[0]) & 0xFFFFFFFF;
        H[1] = (H[1] + h[1]) & 0xFFFFFFFF;
        H[2] = (H[2] + h[2]) & 0xFFFFFFFF;
        H[3] = (H[3] + h[3]) & 0xFFFFFFFF;
        H[4] = (H[4] + h[4]) & 0xFFFFFFFF;
        H[5] = (H[5] + h[5]) & 0xFFFFFFFF;
        H[6] = (H[6] + h[6]) & 0xFFFFFFFF;
        H[7] = (H[7] + h[7]) & 0xFFFFFFFF;
        if (vv) cout << "\t[vv] H register successfully updated..." << endl;
    }


    ///////////////////////////////////////
    // ===== PARSING HASH INTO HEX ===== //
    ///////////////////////////////////////

    if (v) cout << "\t[v] H register ready, preparing hex form..." << endl;
    string hash;

    for (auto x : H) {
        ostringstream oss;
        oss << hex << setw(2) << setfill('0') << (unsigned)x;
        hash += oss.str();
    }
    
    while(hash.length() < 64) hash.insert(0, 1, '0');
    if (v) cout << "\t[v] Hex form ready...\n\t    Outputting..." << endl;
    

    ////////////////////////////
    // ===== OUTPUTTING ===== //
    ////////////////////////////

    if (c){
        cout << endl << "Expected:\t" << compare << endl;
        cout << "Calculated:\t" << hash << endl;
        cout << endl << "\tResult: " << (compare.compare(hash) == 0 ? "MATCH" : "INCORRECT") << endl;
        if(!out_path.empty()){
            if (vv) cout << "\t[vv] Opening " << out_path << " for writing..." << endl;
            ofstream output(out_path);
            if (v) cout << "\t[v] Opened " << out_path << " successfully..." << endl;
            output << hash;
            if (v) cout << "\t[v] Writing successful...\n\t    Closing " << out_path << "..." << endl;
            output.close();
            if (vv) cout << "\t[vv] Closed " << out_path << " successfully..." << endl;
        }
    }
    else {
        if(out_path.empty()){
            if (v) cout << "\t[v] Calculated hash:" << endl;
            cout << hash << endl;
        }
        else {
            if (vv) cout << "\t[vv] Opening " << out_path << " for writing..." << endl;
            ofstream output(out_path);
            if (v) cout << "\t[v] Opened " << out_path << " successfully..." << endl;
            output << hash;
            if (v) cout << "\t[v] Writing successful...\n\t    Closing " << out_path << "..." << endl;
            output.close();
            if (vv) cout << "\t[vv] Closed " << out_path << " successfully..." << endl;
        }
    }

    return 0;
}