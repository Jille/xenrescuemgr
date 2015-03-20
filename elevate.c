#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sysexits.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

#define CONFFILE "/etc/xenrescuemgr.conf"
#define TOOLSTACK_PATH "/usr/sbin/xl"

void
usage(const char *argv0) {
	fprintf(stderr, "Usage: %s <action>\n", argv0);
	exit(EX_USAGE);
}

int
handle_config(const struct passwd *usr, const char *search) {
	FILE *fh;

	int found = 0;

	fh = fopen(CONFFILE, "r");
	if(fh == NULL) {
		err(EX_NOINPUT, "Failed to open config file");
	}
	while(!feof(fh) && !ferror(fh) && !(found && search)) {
		char *user, *domain;
		if(fscanf(fh, "%ms %ms\n", &user, &domain) != 2) {
			break;
		}
		if(strcmp(user, usr->pw_name) == 0) {
			if(search == NULL) {
				puts(domain);
				found = 1;
			} else if(strcmp(domain, search) == 0) {
				found = 1;
			}
		}
		free(user);
		free(domain);
	}
	if(ferror(fh)) {
		errx(EX_NOINPUT, "Error while reading from configfile");
	}
	fclose(fh);
	return found;
}

int
main(int argc, char **argv) {
	struct passwd *usr;

	if(argc < 2) {
		usage(argv[0]);
	}

	usr = getpwuid(getuid());
	if(usr == NULL) {
		errx(EX_OSERR, "Who are you?");
	}

	if(setreuid(0, 0) != 0) {
		err(EX_NOPERM, "Privilege elevation failed");
	}

	if(strcmp(argv[1], "list") == 0) {
		handle_config(usr, NULL);
	} else if(strcmp(argv[1], "console") == 0 || strcmp(argv[1], "domid") == 0 || strcmp(argv[1], "reboot") == 0 || strcmp(argv[1], "reset") == 0 || strcmp(argv[1], "destroy") == 0 || strcmp(argv[1], "info") == 0) {
		if(argc != 3) {
			usage(argv[0]);
		}
		if(!handle_config(usr, argv[2])) {
			err(EX_NOPERM, "Permission denied");
		}
		execl(TOOLSTACK_PATH, TOOLSTACK_PATH, argv[1], argv[2], NULL);
		err(EX_OSERR, "Failed to exec");
	// XXX sysrq
	// XXX create (-c)
	} else {
		usage(argv[0]);
	}

	return 0;
}
