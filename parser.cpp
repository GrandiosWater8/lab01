#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <any>
#include <sstream>

using namespace std;

class JSON
{
private:
    
    map<string, any> Map;//string-ключ
    vector <any> Vector;
    
public:
    
    JSON() {}
    JSON(const string& s)
    {
        Parse(s);
    }
    
    void Parse(const string& s)
    {
        parse(s);
    }
    void ParseFile(const string& path_to_file)//читает файл
    {
        string File;
        ifstream input(path_to_file);
        while (input)
        {
            string a;
            input >> a;
            File += a;
        }
        parse(File);
    }
    
    void parse(const string& str)
    {
        int pos = 0;
        while ((str[pos] != '{') && (str[pos] != '[')) pos++;
        if (str[pos] == '{')//объект содержит ключ
        {
            string key, value;
            pos++;
            while (str[pos] != '}')
            {
                while (str[pos] != '"') pos++;
                key = parse_key(str, pos);
                while (str[pos] != '"' && str[pos] != '[' && str[pos] != 'f' && str[pos] != 't' && str[pos] != 'n' && str[pos] != '{' && str[pos] != '0' && str[pos] != '1' && str[pos] != '2' && str[pos] != '3' && str[pos] != '4' && str[pos] != '5' && str[pos] != '6' && str[pos] != '7' && str[pos] != '8' && str[pos] != '9') pos++;
                if (str[pos] == '"')//слово
                {
                    pos++;
                    while (str[pos] != '"')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += '"';
                    pos++;
                    Map[key] = parse_string(value);
                }
                if (str[pos] == '[')//массив
                {
                    pos++;
                    value += '[';
                    while (str[pos] != ']')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += ']';
                    pos++;
                    Map[key] = parse_array(value);
                }
                if (str[pos] == '{')
                {
                    pos++;
                    value += '{';
                    while (str[pos] != '}')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += '}';
                    pos++;
                    Map[key] = parse_object(value);
                }
                if (str[pos] == 'f')
                {
                    pos += 5;
                    Map[key] = false;
                }
                if (str[pos] == 't')
                {
                    pos += 4;
                    Map[key] = true;
                }
                if (str[pos] == 'n')
                {
                    pos += 4;
                    Map[key] = NULL;
                }
                if (str[pos] == '0' || str[pos] == '1' || str[pos] == '2' || str[pos] == '3' || str[pos] == '4' || str[pos] == '5' || str[pos] == '6' || str[pos] == '7' || str[pos] == '8' || str[pos] == '9')
                {
                    while (str[pos] != ' ' && str[pos] != ',')
                    {
                        value += str[pos];
                        pos++;
                    }
                    Map[key] = parse_int(value);
                }
                value = "";
            }
        }
        if (str[pos] == '[')//массив
        {
            string value;
            pos++;
            while (str[pos] != ']')
            {
                while (str[pos] != '"' && str[pos] != '[' && str[pos] != 'f' && str[pos] != 't' && str[pos] != 'n' && str[pos] != '{' && str[pos] != '0' && str[pos] != '1' && str[pos] != '2' && str[pos] != '3' && str[pos] != '4' && str[pos] != '5' && str[pos] != '6' && str[pos] != '7' && str[pos] != '8' && str[pos] != '9') pos++;
                if (str[pos] == '"')
                {
                    pos++;
                    while (str[pos] != '"')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += '"';
                    pos++;
                    Vector.push_back(parse_string(value));
                }
                if (str[pos] == '[')
                {
                    pos++;
                    value += '[';
                    while (str[pos] != ']')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += ']';
                    pos++;
                    Vector.push_back(parse_array(value));
                }
                if (str[pos] == '{')
                {
                    pos++;
                    value += '{';
                    while (str[pos] != '}')
                    {
                        value += str[pos];
                        pos++;
                    }
                    value += '}';
                    pos++;
                    Vector.push_back(parse_object(value));
                }
                if (str[pos] == 'f')
                {
                    pos += 5;
                    Vector.push_back(false);
                }
                if (str[pos] == 't')
                {
                    pos += 4;
                    Vector.push_back(true);
                }
                if (str[pos] == 'n')
                {
                    pos += 4;
                    Vector.push_back(NULL);
                }
                if (str[pos] == '0' || str[pos] == '1' || str[pos] == '2' || str[pos] == '3' || str[pos] == '4' || str[pos] == '5' || str[pos] == '6' || str[pos] == '7' || str[pos] == '8' || str[pos] == '9')
                {
                    while (str[pos] != ' ' && str[pos] != ',')
                    {
                        value += str[pos];
                        pos++;
                    }
                    Vector.push_back(parse_int(value));
                }
            }
        }
    }
    string parse_key(const string& str, int& pos)//парсим ключ
    {
        string strret;
        pos++;
        while (str[pos] != '"')
        {
            strret += str[pos];
            pos++;
        }
        pos++;
        return strret;
    }
    
    // парсер значения
    string parse_string(const string& str)
    {
        int pos = 0;
        string strret;
        while (str[pos] != '"')
        {
            strret += str[pos];
            pos++;
        }
        return strret;
    }
    
    int parse_int(const string& str)
    {
        string numstr;
        int num;
        for (auto i : str)
        {
            numstr += i;
        }
        stringstream stream;
        stream << numstr;
        stream >> num;
        return num;
    }
    map<string, any> parse_object(const string& str)
    {
        map<string, any> M;
        int pos = 1;
        string key, value;
        while (str[pos] != '}')
        {
            while (str[pos] != '"') pos++;
            key = parse_key(str, pos);
            while (str[pos] != '"' && str[pos] != '[' && str[pos] != 'f' && str[pos] != 't' && str[pos] != 'n' && str[pos] != '{' && str[pos] != '0' && str[pos] != '1' && str[pos] != '2' && str[pos] != '3' && str[pos] != '4' && str[pos] != '5' && str[pos] != '6' && str[pos] != '7' && str[pos] != '8' && str[pos] != '9') pos++;
            if (str[pos] == '"')
            {
                pos++;
                while (str[pos] != '"')
                {
                    value += str[pos];
                    pos++;
                }
                value += '"';
                pos++;
                M[key] = parse_string(value);
            }
            if (str[pos] == 'f')
            {
                pos += 5;
                M[key] = false;
            }
            if (str[pos] == 't')
            {
                pos += 4;
                M[key] = true;
            }
            if (str[pos] == 'n')
            {
                pos += 4;
                M[key] = NULL;
            }
            if (str[pos] == '0' || str[pos] == '1' || str[pos] == '2' || str[pos] == '3' || str[pos] == '4' || str[pos] == '5' || str[pos] == '6' || str[pos] == '7' || str[pos] == '8' || str[pos] == '9')
            {
                while (str[pos] != ' ' && str[pos] != ',')
                {
                    value += str[pos];
                    pos++;
                }
                M[key] = parse_int(value);
            }
            value = "";
        }
        return M;
    }
    
    vector <any> parse_array(const string& str)
    {
        int pos = 1;
        string value;
        vector <any> V;
        while (str[pos] != ']')
        {
            while (str[pos] != ',' && str[pos] != ']')
            {
                value += str[pos];
                pos++;
            }
            if (str[pos] != ']') pos++;
            if (is_string(value)) V.push_back(parse_string(value));
            if (is_int(value)) V.push_back(parse_int(value));
            if (is_true(value)) V.push_back(true);
            if (is_false(value)) V.push_back(false);
            if (is_null(value)) V.push_back(NULL);
            value = "";
            if (str[pos] == ']') break;
        }
        return V;
    }
    
    
    // проверка
    bool is_array(const string& s) const
    {
        if (s[0] == '[') return true;
        return false;
    }
    
    bool is_object(const string& s) const
    {
        if (s[0] == '{') return true;
        return false;
    }
    
    bool is_string(const string& s) const
    {
        if (s[0] == '"') return true;
        return false;
    }
    
    bool is_false(const string& s) const
    {
        if (s[0] == 'f') return true;
        return false;
    }
    
    bool is_true(const string& s) const
    {
        if (s[0] == 't') return true;
        return false;
    }
    
    bool is_null(const string& s) const
    {
        if (s[0] == 'n') return true;
        return false;
    }
    
    bool is_int(const string& s) const
    {
        if ((s[0] == '0') || (s[0] == '1') || (s[0] == '2') || (s[0] == '3') || (s[0] == '4') || (s[0] == '5') || (s[0] == '6') || (s[0] == '7') || (s[0] == '8') || (s[0] == '9')) return true;
        else return false;
    }
    
    bool is_array()
    {
        if (Vector.empty() == true) return false;
        else return true;
    }
    
    bool is_object()
    {
        if (Vector.empty() == true) return true;
        else return false;
    }
    
    // вывод
    void operator[] (const string& key)
    {
        try
        {
            cout << any_cast<string>(Map[key]);
        }
        catch (const bad_any_cast& e) {}
        try
        {
            cout << any_cast<int>(Map[key]);
        }
        catch (const bad_any_cast& f) {}
        try
        {
            cout << any_cast<bool>(Map[key]);
        }
        catch (const bad_any_cast& h) {}
        try
        {
            vector<int> a = any_cast<vector<int>>(Map[key]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast& l) {}
        try
        {
            vector<string> a = any_cast<vector<string>>(Map[key]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast& k) {}
        try
        {
            vector<bool> a = any_cast<vector<bool>>(Map[key]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast& j) {}
        try
        {
            map <string, string> a = any_cast<map<string, string>>(Map[key]);
            for (auto i : a) cout << i.first << " " << i.second << endl;
        }
        catch (const bad_any_cast& g) {}
        try
        {
            map <string, int> a = any_cast<map<string, int>>(Map[key]);
            for (auto i : a) cout << i.first << " " << i.second << "   " << endl;
        }
        catch (const bad_any_cast& z) {}
        try
        {
            map <string, bool> a = any_cast<map<string, bool>>(Map[key]);
            for (auto i : a) cout << i.first << " " << i.second << endl;
        }
        catch (const bad_any_cast& c) {}
    }
    
    void operator [](int index)
    {
        try
        {
            cout << any_cast<string>(Vector[index]);
        }
        catch (const bad_any_cast& a) {}
        try
        {
            cout << any_cast<int>(Vector[index]);
        }
        catch (const bad_any_cast& b) {}
        try
        {
            cout << any_cast<bool>(Vector[index]);
        }
        catch (const bad_any_cast& c) {}
        try
        {
            vector<string> a = any_cast<vector<string>>(Vector[index]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast& d) {}
        try
        {
            vector<int> a = any_cast<vector<int>>(Vector[index]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast e) {};
        try
        {
            vector<bool> a = any_cast<vector<bool>>(Vector[index]);
            for (auto i : a) cout << i << " ";
        }
        catch (const bad_any_cast& f) {}
        try
        {
            map<string, string> a = any_cast<map<string, string>>(Vector[index]);
            for (auto i : a) cout << i.first << " " << i.second << "   ";
        }
        catch (const bad_any_cast & g) {}
        try
        {
            map<string, int> a = any_cast<map<string, int>>(Vector[index]);
            for (auto i : a) cout << i.first << " " << i.second << "   ";
        }
        catch (const bad_any_cast& h) {}
        try
        {
            map<string, bool> a = any_cast<map<string, bool>>(Vector[index]);
            for (auto i : a) cout << i.first << " " << i.second << "   ";
        }
        catch (const bad_any_cast& t) {}
    }
    
};

int main()
{
    JSON a;
    a.ParseFile("Text.txt");
    a["firstname"];
    cout << endl;
    a["age"];
    cout << endl;
    a["islegal"];
    cout << endl << a.is_array() << " " << a.is_object();
}
