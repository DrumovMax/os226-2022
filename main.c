#include <stdio.h>
#include <string.h>
#include <pool.h>


#define MAX_INPUT_LENGTH 512
#define MAX_NUMBER_ARGS 64

int RETCODE = 0;

int echo(int argc, char *argv[]) 
{
	for (int i = 1; i < argc; ++i) 
	{
		printf("%s%c", argv[i], i == argc - 1 ? '\n' : ' ');
	}
	return argc - 1;
}

int retcode(int argc, char *argv[]) 
{
	printf("%d\n", RETCODE);
	return 0;
}

void runCommand(int count, char* args[])
{
	if(strncmp(args[0], "echo", 4) == 0)
	{
		RETCODE = echo(count, args);
	}
	else if(strncmp(args[0], "retcode", 7) == 0)
	{
		RETCODE = retcode(count, args);
	}
}

void execute(char* input)
{
	char* args_str;
	char* arg;

	char* args_ptr;
	char* arg_ptr;

	char* args[MAX_NUMBER_ARGS];

	int counter = 0;

	args_str = strtok_r(input, ";\n", &args_ptr);

	while (args_str)
	{
		arg = strtok_r(args_str, " \n", &arg_ptr);

		while (arg != NULL)
		{
			args[counter] = arg;
			arg = strtok_r(NULL, " \n", &arg_ptr);

			counter++;
		}

		runCommand(counter, args);

		counter = 0;
		args_str = strtok_r(NULL, ";\n", &args_ptr);
	}
}

static int pooltest(int argc, char *argv[]) {
	struct obj {
		void *field1;
		void *field2;
	};
	static struct obj objmem[4];
	static struct pool objpool = POOL_INITIALIZER_ARRAY(objmem);

	if (!strcmp(argv[1], "alloc")) {
		struct obj *o = pool_alloc(&objpool);
		printf("alloc %d\n", o ? (o - objmem) : -1);
		return 0;
	} else if (!strcmp(argv[1], "free")) {
		int iobj = atoi(argv[2]);
		printf("free %d\n", iobj);
		pool_free(&objpool, objmem + iobj);
		return 0;
	}
}

int main(int argc, char *argv[]) 
{
	char input[MAX_INPUT_LENGTH];

	while (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL)
	{
		execute(input);
	}

	return 0;
}
