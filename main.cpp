#include <iostream>//Stream IO.
#include <fstream>//File IO.
#include <string>
#include <vector>//Para crear vectores de string principalmente.
#include <windows.h>//Para consultar los procesos con ventana abierta y para la música de los créditos.
#include <thread>//Para escribir a la vez que se reproduce la música.
#include <map>//Para mapear el nombre de la persona con la frase dedicada a ella.
#pragma comment(lib, "winmm.lib")//Para la música de los créditos.

using namespace std;

struct WindowInfo {
	wstring windowTitle;
	wstring processName;
};

vector<WindowInfo> windowInfoList;//Variable global, no hay otro modo si no podemos tocar qué retorna ni los parámetros de la función EnumWindowsProc(...)

void inicializarContadores(unsigned long long int tiempoPrograma[], const size_t nProgramas);
bool contains(string o, string s);//Le pasamos el output y hacemos find directamente.
bool contains(vector<WindowInfo> vectorWI, string s);
bool anadirPrograma(string p);
void eliminarPrograma(string p);
bool existePrograma(string p);
void menu();
void escribirCreditos();
vector<string> getProgramasArchivo();
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
wstring nombreProceso(wstring cadenaCompleta);

int main() {
	const int espera = 10000;//Valor en ms de intervalo de tiempo.
	vector<string> programasArchivo;//Almacena los programas leídos en el archivo listaProgramas.txt.
	EnumWindows(EnumWindowsProc, NULL);

	menu();

	programasArchivo = getProgramasArchivo();//Almacenamos los programas leídos en el archivo en programasArchivo.
	const size_t nProgramasArchivo = programasArchivo.size();//Almacenamos el número de programas leídos en nProgramasArchivo. Almacena el número de programas del archivo listaProgramas.txt.
	unsigned long long int tiempoPrograma[10] = {};//Declaramos un array para el tiempo de cada programa. Como Visual Studio no nos deja declarar el tamaño del array en función del nº de programas, pondremos 10 por ejemplo.
	unsigned _int64 segundos = 0, minutos = 0, horas = 0;//Estas variables almacenarán los valores temporales de la medición del tiempo.

	SYSTEMTIME st;//Variable que nos sirve para almacenar la fecha actual gracias a la Windows API.
	map<int, string> nombreMes = {
		{1,"enero" },
		{2,"febrero" },
		{3,"marzo" },
		{4,"abril" },
		{5,"mayo" },
		{6,"junio" },
		{7,"julio" },
		{8,"agosto" },
		{9,"septiembre" },
		{10,"octubre" },
		{11,"noviembre" },
		{12,"diciembre" },
	};
	int diaActual = 0;//Almacenará el día actual. En caso de que cambie, le asignaremos 0 a los contadores, ha empezado un nuevo día.

	while (true) {
		windowInfoList.clear();
		EnumWindows(EnumWindowsProc, NULL);
		system("cls");

		/*cout << "nProgramas archivo vale " << nProgramasArchivo << endl;
		system("pause");*/

		GetLocalTime(&st);
		if (st.wDay != diaActual) {//Si cambia el día...
			diaActual = st.wDay;//El día actual será otro...
			inicializarContadores(tiempoPrograma,nProgramasArchivo);//Y los contadores se reiniciarán.
		}
		cout << "A dia " << diaActual << " de " << nombreMes[st.wMonth] << endl;
		for (int i = 0; i < nProgramasArchivo; i++) {
			segundos = tiempoPrograma[i] / 1000;
			if (segundos < 60) {
				cout << "El programa " << programasArchivo[i] << " lleva abierto " << segundos << " s" << endl;
			}
			else {
				minutos = tiempoPrograma[i] / (1000 * 60);
				if (minutos < 60) {
					segundos = (tiempoPrograma[i] / 1000) - (minutos * 60);
					cout << "El programa " << programasArchivo[i] << " lleva abierto " << minutos << " min " << segundos << " s" << endl;
				}
				else {
					horas = tiempoPrograma[i] / (1000 * 60 * 60);
					//No hay if(horas<24) porque reiniciaremos cada día el contador del tiempo.
					minutos = tiempoPrograma[i] / (1000 * 60) - (horas * 60);
					cout << "El programa " << programasArchivo[i] << " lleva abierto " << horas << " h " << minutos << " min" << endl;
				}
			}
		}

		for (int i = 0; i < nProgramasArchivo; i++) {
			if (contains(windowInfoList, programasArchivo[i])) {
				tiempoPrograma[i] += espera;
			}
		}

		Sleep(espera);
	}
	return 0;
}
//Inicializa todos los contadores a 0.
void inicializarContadores(unsigned long long int tiempoPrograma[],const size_t nProgramas) {
	for (int i = 0; i < nProgramas; i++) {
		tiempoPrograma[i] = 0;
	}
}
/*Añade el programa pasado por parámetro al archivo.
Retorna true si se ha añadido correctamente, false en caso contrario.
Se preocupa por no añadir programas ya insertados en el archivo.
*/
bool anadirPrograma(string p) {
	ofstream escritura;
	bool exito = false;
	if (!existePrograma(p)) {
		escritura.open("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt", ios::app);//Con ios::app respetamos el contenido previo y no sobreescribimos.
		if (escritura.is_open()) {
			escritura << p << endl;
			escritura.close();
			exito = true;
		}
	}
	escritura.close();//Siempre cerraremos los archivos al final del código, aunque no hayamos hecho open de ellos. Esto hace que evitamos errores a la hora de cerrar o no los archivos.
	return exito;
}
//Elimina el programa pasado por parámetro del fichero. Lo hace guardando todo en strings menos la string pasada.
void eliminarPrograma(string p) {
	ifstream archivo;
	ofstream archivo2;
	vector<string> programasArchivo;//Guardaré todos los programas menos la string pasada en este vector.
	string programa;//Programa que se va almacenando temporalmente en cada iteración.
	bool eliminado = false;//Si se ha eliminado correctamente el archivo, valdrá true.

	if (!existePrograma(p)) {
		cout << "Error: no existe el programa que desea eliminar.";
	}
	else {
		archivo.open("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt");
		if (archivo.is_open()) {
			while (archivo.eof() == false) {
				getline(archivo, programa, '\n');//Lee línea a línea el archivo y lo guarda en programa en cada iteración.
				//if(programa != "") cout<<"Programa:"<<programa<<endl;
				if (programa.find(p) != string::npos) {
					//Si lo encontramos no debemos hacer nada.
				}
				else {
					if (programa != "") {
						//cout<<"Anhadimos "<<programa<<endl;
						programasArchivo.push_back(programa);//Si no lo encontramos, lo almacenamos en el vector.
					}
				}
			}
			archivo.close();//Cerramos el archivo antes de eliminarlo.
			eliminado = !remove("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt");
			if (eliminado) {
				//cout<<"Se ha eliminado correctamente el archivo, se procede a crear uno nuevo"<<endl;
				archivo2.open("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt");
				if (archivo2.is_open()) {
					for (int i = 0; i < programasArchivo.size(); i++) {
						archivo2 << programasArchivo[i] << endl;
					}
				}
			}
		}
	}
	archivo2.close();
}
//Retorna true si el programa pasado por parámetro existe en el archivo listaProgramas.txt.
bool existePrograma(string p) {
	ifstream lectura;
	bool existe = false;
	string programa = "";
	lectura.open("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt");
	if (lectura.is_open()) {
		while (lectura.eof() == false) {
			getline(lectura, programa, '\n');//Lee línea a línea el archivo y lo guarda en programa en cada iteración.
			if (programa == p) {
				existe = true;
			}
		}
	}
	lectura.close();
	return existe;
}
bool contains(string o, string s) {
	if (o.find(s) != string::npos) {
		return true;
	}
	else {
		return false;
	}
}
bool contains(vector<WindowInfo> vectorWI, string s) {
	bool encontrado = false;
	for (int i = 0; i < vectorWI.size(); i++) {
		string processNameString(vectorWI[i].processName.begin(), vectorWI[i].processName.end());
		if (processNameString.find(s) != string::npos) {
			encontrado = true;
		}
	}
	return encontrado;
}
void menu() {
	char input[2] = { 0,0 };
	string programaDeseado;
	bool encontrado = false;
	vector<string> programasAnhadidos;//Vector que almacena los programas abiertos al momento de ir a la opción de añadir programas.
	wstring nombrePrograma;//Variable que irá recogiendo los nombres procesados de cada programa para el usuario.
	int pos_Exe = 0;//Posición desde donde empezaremos a truncar.
	bool arranqueMargaret = false;
	system("cls");
	do {
		cout << "Bienvenido a Margaret. Pulse la opcion deseada" << endl;
		cout << "1. Encendido de Margaret" << endl;
		cout << "2. Gestion de programas monitorizados" << endl;
		cout << "3. Creditos y agradecimientos" << endl;
		cin >> input[0];//Antes lo hacía con getch().
		system("cls");
		switch (input[0]) {
		case '1': {
			arranqueMargaret = true;
			cout << "Iniciando Margaret" << endl;
			system("pause");
			system("cls");
		}
			  break;
		case '2': {
			do {
				system("cls");
				cout << "Gestion de programas monitorizados" << endl;
				cout << "Pulse la opcion deseada" << endl;
				cout << "1. Anhadir programa" << endl;
				cout << "2. Eliminar programa" << endl;
				cout << "3. Salir al menu principal" << endl;
				cin >> input[1];//Antes lo hacía con getch().
				switch (input[1]) {
				case '1': {
					windowInfoList.clear();//Actualizamos windowInfoList para cuando abra la opción.
					EnumWindows(EnumWindowsProc, NULL);
					system("cls");
					cout << "Programas abiertos actualmente" << endl;
					for (int i = 0; i < windowInfoList.size(); i++) {
						wcout << nombreProceso(windowInfoList[i].processName)<<endl;
					}
					cin.ignore();
					cout << "Introduzca el nombre del programa a anhadir (q para salir):";
					getline(cin, programaDeseado);
					if (programaDeseado == "q") {
						system("cls");
					}
					else {
						if (anadirPrograma(programaDeseado) == true) {
							cout << "Programa anhadido correctamente" << endl;
							system("pause");
							system("cls");
						}
						else {
							system("cls");
							cout << "Error: recuerde no repetir el programa" << endl;
							input[1] = '\0';
							system("pause");
						}
					}
					
				}
					  break;
				case '2': {
					system("cls");
					cin.ignore();
					programasAnhadidos = getProgramasArchivo();
					if (programasAnhadidos.size() == 0) {
						cout << "No hay programas anhadidos" << endl;
						system("pause");
					}
					else {
						cout << "Programas anhadidos" << endl;
						for (int i = 0; i < programasAnhadidos.size(); i++) {
							cout << programasAnhadidos[i] << endl;
						}
						cout << "Introduzca el programa a eliminar (q para salir):";
						getline(cin, programaDeseado);
						if (programaDeseado == "q") {
							system("cls");
						}
						else {
							encontrado = existePrograma(programaDeseado);
							if (!encontrado) {
								system("cls");
								cout << "Error: el programa indicado no existe en el archivo." << endl;
								input[1] = '\0';
								system("pause");
							}
							else {
								eliminarPrograma(programaDeseado);
								cout << "Programa eliminado con exito" << endl;
								system("pause");
								system("cls");
							}
						}
					}
				}
					  break;
				case '3': {
					system("cls");
					programaDeseado = "q";
				}
						break;
				default: {
					system("cls");
					cout << "Error: introduzca un valor valido" << endl;
					system("pause");
					system("cls");
				}
				}
			} while ((input[1] < '1' || input[1]> '3') && (programaDeseado != "q"));
		}
			  break;
		case '3': {
			thread t1(escribirCreditos);
			PlaySound(TEXT("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/Margaret/musicaCreditos.wav"), 0, SND_FILENAME);
			t1.join();
			system("cls");
		}
			  break;
		default: {
			system("cls");
			cout << "Error: introduzca un valor valido" << endl;
			system("pause");
			system("cls");
		}
		}
	} while (!arranqueMargaret);//while (input[0] < 1 || input[0] > 3 || (input[1] == 0 && (input[0] < 1 || input[0] > 3)));
}

// Devuelve un vector con los strings que buscara los procesos
void escribirCreditos() {
	vector<string> nombre = {"Persona 1","Persona 2","Persona 3","Persona 4","Persona 5"};
	map<string, string> personicas = {
		{"Persona 1","Por..."},
		{"Persona 2","Por..."},
		{"Persona 3","Por..."},
		{"Persona 4","Por..."},
		{"Persona 5","Por..."},
	};
	cout << "Programa realizado por Jesus Garcia Gutierrez." << endl;
	cout << "Margaret no existiria sin el apoyo de estas personas, GRACIAS." << endl;
	for (int i = 0; i < 62; i++) {
		cout << "-";
		Sleep(100);
	}
	cout << endl;
	for (int i = 0; i < personicas.size(); i++) {
		cout << nombre[i] << endl;
		for (int j = 0; j < personicas[nombre[i]].length(); j++) {
			cout << personicas[nombre[i]][j];
			Sleep(100);
		}
		cout << endl;
	}
}
vector<string> getProgramasArchivo() {
	//Lee los programas del archivo y los retorna en un vector<string
	vector<string> programasArchivo;
	string programa;
	ifstream lectura;
	lectura.open("C:/Users/jesus/Documents/C++/ProyectosVisualStudio/Margaret/x64/Debug/listaProgramas.txt");
	if (lectura.is_open()) {
		while (lectura.eof() == false) {
			lectura >> programa;
			programasArchivo.push_back(programa);
		}
		programasArchivo.pop_back();//Para eliminar la última string, que es igual en la penúltima posición que en la última.
		lectura.close();
	}
	return programasArchivo;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	WindowInfo info;
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	TCHAR buffer[MAX_PATH];
	GetWindowText(hwnd, buffer, MAX_PATH);
	//wcout << "Window title: " << buffer << endl;
	info.windowTitle = buffer;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (hProcess != NULL) {
		TCHAR processName[MAX_PATH];
		DWORD processNameSize = MAX_PATH;
		if (QueryFullProcessImageName(hProcess, 0, processName, &processNameSize)) {
			//wcout << "Process name: " << processName << endl;
			info.processName = processName;
		}
		CloseHandle(hProcess);
	}
	wcout << endl;
	bool existe = false;
	for (int i = 0; i < windowInfoList.size(); i++) {
		if (info.processName == windowInfoList[i].processName) {
			existe = true;
			break;
		}
	}
	if (!existe) {
		if (IsWindowVisible(hwnd)) {
			windowInfoList.push_back(info);
		}
	}
	return TRUE;
}
//A esta función le pasamos la cadena completa del proceso (C\..\proceso.exe) y nos devolverá el nombre del proceso.
wstring nombreProceso(wstring cadenaCompleta) {
	int pos = cadenaCompleta.find_last_of('\\') + 1;/*La posición de la primera letra del proceso será la de justo después
													 de la última barra. Como es back slash hace falta otra back slash.*/
	bool parar = false;
	wstring nProceso;
	while (parar == false) {
		if (cadenaCompleta[pos] == '.') {
			parar = true;
		}
		else {
			nProceso += cadenaCompleta[pos];
			pos++;
		}
	}
	return nProceso;
}