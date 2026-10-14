#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;

// Separador usado para guardar los campos en el archivo
const char SEPARADOR = '|';

// ---------------------------------------------------------------
// Encriptador: cifrado por sustitucion simple (SOLO EDUCATIVO).
// No es seguro: la clave esta escrita en el codigo.
// ---------------------------------------------------------------
class Encriptador {
private:
    string cipherAlphabet = "qwertyuiopasdfghjklzxcvbnm";
    string plainAlphabet  = "abcdefghijklmnopqrstuvwxyz";

    bool Cipher(const string& input, const string& oldAlphabet,
                const string& newAlphabet, string& output) {
        output = "";
        for (char c : input) {
            size_t index = oldAlphabet.find(
                static_cast<char>(tolower(static_cast<unsigned char>(c))));
            if (index != string::npos) {
                output += isupper(static_cast<unsigned char>(c))
                              ? static_cast<char>(toupper(newAlphabet[index]))
                              : newAlphabet[index];
            } else {
                output += c; // numeros y simbolos no se modifican
            }
        }
        return true;
    }

public:
    bool encriptar(const string& input, string& output) {
        return Cipher(input, plainAlphabet, cipherAlphabet, output);
    }

    bool desencriptar(const string& input, string& output) {
        return Cipher(input, cipherAlphabet, plainAlphabet, output);
    }
};

// ---------------------------------------------------------------
// PasswordEntry: representa una entrada (url, password, usuario, notas)
// ---------------------------------------------------------------
class PasswordEntry {
private:
    string url;
    string password;
    string username;
    string notas;

public:
    PasswordEntry(const string& url, const string& password,
                  const string& username, const string& notas)
        : url(url), password(password), username(username), notas(notas) {}

    string getUrl() const { return url; }
    string getUsername() const { return username; }
    string getNotas() const { return notas; }

    string getPassword(bool decrypt = false, Encriptador* enc = nullptr) const {
        if (decrypt && enc) {
            string decrypted;
            if (enc->desencriptar(password, decrypted)) {
                return decrypted;
            }
        }
        return password;
    }

    void mostrar(bool showDecrypted = false, Encriptador* enc = nullptr) const {
        cout << "URL: " << url << "\n";
        cout << "Username: " << username << "\n";
        cout << "Password: " << getPassword(showDecrypted, enc) << "\n";
        cout << "Notas: " << notas << "\n";
        cout << "----------------------------\n";
    }

    bool toFileString(string& output) const {
        output = url + SEPARADOR + password + SEPARADOR + username + SEPARADOR + notas;
        return true;
    }

    static bool fromFileString(const string& line, PasswordEntry& entry) {
        size_t pos1 = line.find(SEPARADOR);
        if (pos1 == string::npos) return false;
        size_t pos2 = line.find(SEPARADOR, pos1 + 1);
        if (pos2 == string::npos) return false;
        size_t pos3 = line.find(SEPARADOR, pos2 + 1);
        if (pos3 == string::npos) return false;

        entry = PasswordEntry(
            line.substr(0, pos1),
            line.substr(pos1 + 1, pos2 - pos1 - 1),
            line.substr(pos2 + 1, pos3 - pos2 - 1),
            line.substr(pos3 + 1)
        );
        return true;
    }
};

// ---------------------------------------------------------------
// PasswordManager: administra la lista y el archivo
// ---------------------------------------------------------------
class PasswordManager {
private:
    vector<PasswordEntry> entries;
    string filename;
    Encriptador encriptador;

    bool cargarDesdeArchivo() {
        ifstream file(filename);
        if (!file) {
            cerr << "Archivo no encontrado. Se creara uno nuevo al guardar.\n";
            return true;
        }

        string line;
        while (getline(file, line)) {
            PasswordEntry entry("", "", "", "");
            if (PasswordEntry::fromFileString(line, entry)) {
                entries.push_back(entry);
            }
        }
        return true;
    }

    bool guardarEnArchivo() {
        ofstream file(filename);
        if (!file) {
            cerr << "Error al guardar el archivo.\n";
            return false;
        }

        for (const auto& entry : entries) {
            string line;
            if (entry.toFileString(line)) {
                file << line << "\n";
            }
        }
        return true;
    }

    // El separador romperia el formato del archivo si aparece en un campo
    static bool contieneSeparador(const string& texto) {
        return texto.find(SEPARADOR) != string::npos;
    }

public:
    PasswordManager(const string& filename) : filename(filename) {
        cargarDesdeArchivo();
    }

    ~PasswordManager() {
        guardarEnArchivo();
    }

    bool agregarEntry(const string& url, const string& password,
                      const string& username, const string& notas) {
        if (contieneSeparador(url) || contieneSeparador(password) ||
            contieneSeparador(username) || contieneSeparador(notas)) {
            cout << "Los campos no pueden contener el caracter '" << SEPARADOR << "'.\n";
            return false;
        }

        string encryptedPass;
        if (!encriptador.encriptar(password, encryptedPass)) {
            return false;
        }
        entries.emplace_back(url, encryptedPass, username, notas);
        return true;
    }

    void mostrarTodas(bool showDecrypted = false) {
        if (entries.empty()) {
            cout << "No hay entradas guardadas.\n";
            return;
        }
        for (const auto& entry : entries) {
            entry.mostrar(showDecrypted, showDecrypted ? &encriptador : nullptr);
        }
    }

    vector<PasswordEntry> buscarPorUrl(const string& url) const {
        vector<PasswordEntry> resultados;
        string searchLower = url;
        transform(searchLower.begin(), searchLower.end(), searchLower.begin(),
                  [](unsigned char c) { return static_cast<char>(tolower(c)); });

        for (const auto& entry : entries) {
            string entryUrl = entry.getUrl();
            transform(entryUrl.begin(), entryUrl.end(), entryUrl.begin(),
                      [](unsigned char c) { return static_cast<char>(tolower(c)); });

            if (entryUrl.find(searchLower) != string::npos) {
                resultados.push_back(entry);
            }
        }
        return resultados;
    }

    Encriptador* getEncriptador() {
        return &encriptador;
    }
};

void mostrarMenu() {
    cout << "\nGestor de Passwords\n";
    cout << "1. Agregar nueva entrada\n";
    cout << "2. Mostrar todas las entradas (encriptadas)\n";
    cout << "3. Mostrar contrasenas desencriptadas\n";
    cout << "4. Buscar por URL\n";
    cout << "5. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    PasswordManager manager("passwords.dat");

    int opcion = 0;
    while (opcion != 5) {
        mostrarMenu();

        // Si el usuario escribe algo que no es numero, se limpia el error
        if (!(cin >> opcion)) {
            if (cin.eof()) break; // fin de la entrada
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida. Escribe un numero del 1 al 5.\n";
            opcion = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: {
                string url, password, username, notas;
                cout << "URL: ";
                getline(cin, url);
                cout << "Username: ";
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);
                cout << "Notas: ";
                getline(cin, notas);

                if (manager.agregarEntry(url, password, username, notas)) {
                    cout << "Entrada agregada correctamente.\n";
                } else {
                    cout << "Error al agregar entrada.\n";
                }
                break;
            }
            case 2:
                manager.mostrarTodas(false);
                break;
            case 3:
                manager.mostrarTodas(true);
                break;
            case 4: {
                string url;
                cout << "Ingrese URL o parte de ella a buscar: ";
                getline(cin, url);
                auto resultados = manager.buscarPorUrl(url);
                if (resultados.empty()) {
                    cout << "No se encontraron resultados.\n";
                } else {
                    for (const auto& entry : resultados) {
                        entry.mostrar(true, manager.getEncriptador());
                    }
                }
                break;
            }
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    }

    return 0;
}
