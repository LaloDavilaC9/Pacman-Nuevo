#pragma once

#include <vector>
class A_Search {
	private:
		struct Nodo {
			bool obstaculo = false;
			bool nVisitado = false;
			float fMetaGlobal;
			float FMetaLocal;
			int x, y;
			std::vector <Nodo *> vecN_Vecinos;
			Nodo* padre;
		};
		Nodo *nodos=nullptr;
		int anchoMapa=30;
		int alturaMapa=20;
	protected:
		virtual bool creacionDeUsuario() {return true;}
};