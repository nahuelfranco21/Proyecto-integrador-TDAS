#include "split.h"
#include <stdlib.h>
#include <string.h>

void liberar_vector(char **vector, int cantidad)
{
	if (vector) {
		for (int i = 0; i < cantidad; i++) {
			if (vector[i])
				free(vector[i]);
		}
		free(vector);
	}
}

void liberar_memoria_si_error(char *vector_1, char **vector_2, int cantidad)
{
	free(vector_1);

	for (int i = 0; i < cantidad; i++)
		free(vector_2[i]);

	free(vector_2);
}

char **split(const char *string, char separador)
{
	if (!string)
		return NULL;

	char **vector_strings = NULL;
	char *vector_chars = NULL;

	int contador_string = 0;
	int contador_char = 0;
	int contador_palabras = 0;

	size_t tamanio_string = strlen(string) + 2;

	vector_strings = calloc((tamanio_string), sizeof(char *));
	vector_chars = calloc((tamanio_string), sizeof(char));

	while (string[contador_string] != 0) {
		if (string[contador_string] != separador) {
			vector_chars[contador_char] = string[contador_string];
			contador_char++;
		} else if (string[contador_string] == separador) {
			vector_chars[contador_char] = 0;
			vector_strings[contador_palabras] =
				malloc((tamanio_string) * sizeof(char));
			if (!vector_strings[contador_palabras])
				liberar_memoria_si_error(vector_chars,
							 vector_strings,
							 contador_palabras);
			strcpy(vector_strings[contador_palabras], vector_chars);
			contador_palabras++;
			contador_char = 0;
		}
		contador_string++;
	}

	vector_chars[contador_char] = 0;
	vector_strings[contador_palabras] =
		malloc((tamanio_string) * sizeof(char));

	if (!vector_strings[contador_palabras]) {
		liberar_memoria_si_error(vector_chars, vector_strings,
					 contador_palabras);
		return NULL;
	} else if (vector_strings[contador_palabras] != NULL) {
		strcpy(vector_strings[contador_palabras], vector_chars);
		contador_palabras++;
		vector_strings[contador_palabras] = 0;
	}

	free(vector_chars);

	return vector_strings;
}
