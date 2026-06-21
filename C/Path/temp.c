#include <stdio.h>

#define ALMOG_PATH_MANIPULATION_IMPLEMENTATION
#include "./Almog_Path_Manipulation.h"


int main(int argc, char const *argv[])
{
    const char *entry_file_relative_path = NULL;

    for (int i = 1; i < argc; i++) {
        if (entry_file_relative_path == NULL) {
            entry_file_relative_path = argv[i];
        }
    }
    if (entry_file_relative_path == NULL) {
        apm_dprintERROR("Usage: %s <entry_file>", argv[0]);
        return -1;
    }

    apm_dprintINFO("entry file relative path = %s.", entry_file_relative_path);
    if (APM_FAIL == apm_path_is_valid_file(entry_file_relative_path)) {
        apm_dprintERROR("%s", "Entry file is not a valid file.");
        return -1;
    }

    Apm_Word current_working_directory;
    if (getcwd(current_working_directory, sizeof(current_working_directory)) == NULL) {
        apm_dprintERROR("%s", "Could not get current working directory.");
        return -1;
    }

    Apm_Word entry_file_absolute_path;
    if (APM_SUCCESS == apm_path_is_absolute(entry_file_relative_path)) {
        apm_strncpy(entry_file_absolute_path, entry_file_relative_path, APM_MAX_LEN);
    } else {
        if (APM_FAIL == apm_join_two_paths(entry_file_absolute_path, current_working_directory, (char *)entry_file_relative_path)) {
            apm_dprintERROR("Could not join path2 '%s' to path1 '%s'.", (char *)entry_file_relative_path, current_working_directory);
            return -1;
        }
        if (APM_FAIL == apm_path_is_absolute(entry_file_absolute_path)) {
            apm_dprintERROR("Could not create an absolute path to the entry file. Created: '%s'", entry_file_absolute_path);
            return -1;
        }
    }
    if (APM_FAIL == apm_path_fix(entry_file_absolute_path)) {
        apm_dprintERROR("Could not fix path '%s'", entry_file_absolute_path);
        return -1;
    }
    apm_dprintSTRING(entry_file_absolute_path);

    return 0;
}
