#include <proto/intuition.h>

void AROS_ErrorMessage(const char *message) {
	struct EasyStruct es;

	es.es_StructSize = sizeof(es);
	es.es_Flags = 0;
	es.es_Title = "Aliens vs Predator error";
	es.es_TextFormat = message;
	es.es_GadgetFormat = "Quit";

	EasyRequest(0, &es, 0, 0);
}
