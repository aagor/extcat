/*  Copyright (C) 2015 alcros.
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

    Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1000
#define OK 0
#define FOPEN_ERR 1
#define INPUT_ERROR 2
#define GENERAL_ERROR 3
#define OUTPUT_ERROR 4
#define FCLOSE_ERR 5

void print_version()
{
	puts("extcat 1.0.0\nCopyright (C) 2015 alcros.\nThis is free software; see the source for copying conditions.\nThere is NO \
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}

void print_usage()
{
	print_version();
	puts("\nUsage: extcat file1 file2 file3 ...");
}

void print_error_msg(int msg, char *file)
{
	switch (msg) {
	case OK:
		break;
	case FOPEN_ERR:
		fprintf(stderr, "Fehler beim Oeffnen der Datei %s.\n", file);
		break;
	case INPUT_ERROR:
		fprintf(stderr, "Input Error beim Lesen der Datei %s.\n", file);
		break;
	case GENERAL_ERROR:
		fprintf(stderr, "General Error beim Lesen der Datei %s.\n", file);
		break;
	case OUTPUT_ERROR:
		fprintf(stderr, "Output Error beim Schreiben der Datei %s.\n", file);
		break;
	case FCLOSE_ERR:
		fprintf(stderr, "Fehler beim Schliessen der Datei %s.\n", file);
		break;
	default:
		fputs("Fehler beim Ausgeben des Fehlers.\n", stderr);
		break;
	}
}

int copy_file_to_stdout(char *file)
{
	int readcnt, writecnt;
	char buffer[BUF_SIZE];
	FILE *stream = fopen(file, "r");
	if (stream == NULL)
		return FOPEN_ERR;
	while (1) {
		readcnt = fread(buffer, 1, BUF_SIZE, stream);
		if (readcnt == 0) {
			if (feof(stream))
				return OK;
			else if (ferror(stream))
				return INPUT_ERROR;
			else
				return GENERAL_ERROR;
		}
		writecnt = fwrite(buffer, 1, readcnt, stdout);
		if (writecnt != readcnt)
			return OUTPUT_ERROR;
	}
	if (fclose(stream) == EOF)
		return FCLOSE_ERR;
}

int main(int argc, char **argv)
{
	int i;
	if (argc == 1) {
		print_usage();
		exit(EXIT_FAILURE);
	}
	if (argc == 2 && (!strcmp(argv[1], "--usage") || !strcmp(argv[1], "--help"))) {
		print_usage();
		exit(EXIT_SUCCESS);
	}
	if (argc == 2 && !strcmp(argv[1], "--version")) {
		print_version();
		exit(EXIT_SUCCESS);
	}
	for(i = 1 ; i < argc ; ++i) {
		printf("/* %s */\n", argv[i]);
		print_error_msg(copy_file_to_stdout(argv[i]), argv[i]);
		if (i + 1 < argc)
			puts("\n");
	}
	return EXIT_SUCCESS;
}
