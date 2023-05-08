#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

void sysInfo()
{

    struct utsname unameinfo; // sysinfo struct, its fields will be set by passing a pointer to it in uname()
    uname(&unameinfo);
    printf("### System Information ###\n System Name = %s\n Machine Name = %s\n Version = %s\n Release = %s\n Architecture = %s\n---------------------------------------\n",
           unameinfo.sysname, unameinfo.nodename, unameinfo.version, unameinfo.release, unameinfo.machine);
}

float quick_cpu()
{ // This function is needed in order to give us our first cpu_usage value
    FILE *usage = fopen("/proc/stat", "r");
    if (usage == NULL)
    {
        printf("Unable to open file, terminating program.\n");
        exit(-1);
    }
    char temp[1000];

    long long total_time, total_idle_time, total_usage_time, total_time2, total_idle_time2, total_usage_time2, user_time, nice_time, system_time, idle_time, iowait_time, irq_time, softirq_time, steal_time, guest_time, guest_nice_time;

    fgets(temp, 1000, usage); // Put the first line of the /proc/stat file into temp

    for (int i = 0; i < 1000; i++)
    {
        if (!isdigit(temp[i]) && temp[i] != '\0')
        { // Replace all non digit characters with whitespace so we can use scanf
            temp[i] = ' ';
        }
    }
    // scanf only works with stdin, so we need sscanf if we want the input to be a hardcoded string

    sscanf(temp, " %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", &user_time, &nice_time, &system_time, &idle_time, &iowait_time, &irq_time, &softirq_time, &steal_time, &guest_time, &guest_nice_time);
    total_time = user_time + nice_time + system_time + idle_time + iowait_time + irq_time + softirq_time + steal_time;
    total_idle_time = idle_time + iowait_time;
    total_usage_time = total_time - total_idle_time;

    fclose(usage); // Close and reopen after one second so we get an updated version
    sleep(1);
    FILE *usage_updated = fopen("/proc/stat", "r");

    fgets(temp, 1000, usage_updated);

    for (int i = 0; i < 1000; i++)
    {
        if (!isdigit(temp[i]) && temp[i] != '\0')
        { // Replace all non digit characters with whitespace so we can use scanf
            temp[i] = ' ';
        }
    }

    sscanf(temp, " %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", &user_time, &nice_time, &system_time, &idle_time, &iowait_time, &irq_time, &softirq_time, &steal_time, &guest_time, &guest_nice_time);

    total_time2 = user_time + nice_time + system_time + idle_time + iowait_time + irq_time + softirq_time + steal_time;
    total_idle_time2 = idle_time + iowait_time;
    total_usage_time2 = total_time2 - total_idle_time2;

    long long total_usage_dif = total_usage_time2 - total_usage_time;
    long long total_time_dif = total_time2 - total_time;

    float cpu_usage = (float)total_usage_dif / total_time_dif; // Formula for cpu use % is change in usage time / change in total time
    cpu_usage *= 100;

    fclose(usage_updated);

    return cpu_usage;
}

float cpuInfo(float previous, int delay)
{

    FILE *coreinfo = fopen("/proc/cpuinfo", "r"); // File for calculating # of cores
    if (coreinfo == NULL)
    {
        printf("Unable to open file, terminating program.\n");
        exit(-1);
    }

    char temp[1000]; // String that will store each line of cpuinfo

    while (fgets(temp, 1000, coreinfo) != NULL)
    { // This while loop will break when we reach the line directly above the line that we are intersted in

        if (strstr(temp, "core id") != NULL)
        { // if "core id" appears in temp, break
            break;
        }
    }

    fgets(temp, 1000, coreinfo); // Put the value of the line of interest into temp

    int j;
    for (int i = 0; temp[i] != ':'; i++)
    { // Set j such that the address of ':' in temp is equal to &temp[0] + j + 1
        j = i;
    }
    j = j + 2;                     // Increment by 2 bytes so that &temp[0] + j is the address of the whitespace character after the ':' and before the first digit
    char *core_arr = &temp[0] + j; // Create new string starting at the whitespace before the first digit as explained in the line above so that we can use strtol

    long int cores = strtol(core_arr, NULL, 10);
    printf("\nNumber of cores: %ld\n", cores);
    printf("CPU usage: %f%%\n", previous); // Print the previously generated cpu usage %
    printf("---------------------------------------\n");

    FILE *usage = fopen("/proc/stat", "r");
    if (usage == NULL)
    {
        printf("Unable to open file, terminating program.\n");
        exit(-1);
    }

    long long total_time, total_idle_time, total_usage_time, total_time2, total_idle_time2, total_usage_time2, user_time, nice_time, system_time, idle_time, iowait_time, irq_time, softirq_time, steal_time, guest_time, guest_nice_time;

    fgets(temp, 1000, usage); // Put the first line of the /proc/stat file into temp

    for (int i = 0; i < 1000; i++)
    {
        if (!isdigit(temp[i]) && temp[i] != '\0')
        { // Replace all non digit characters with whitespace so we can use scanf
            temp[i] = ' ';
        }
    }
    // scanf only works with stdin, so we need sscanf if we want the input to be a hardcoded string

    sscanf(temp, " %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", &user_time, &nice_time, &system_time, &idle_time, &iowait_time, &irq_time, &softirq_time, &steal_time, &guest_time, &guest_nice_time);
    total_time = user_time + nice_time + system_time + idle_time + iowait_time + irq_time + softirq_time + steal_time;
    total_idle_time = idle_time + iowait_time;
    total_usage_time = total_time - total_idle_time;

    fclose(usage); // Close and reopen after one second so we get an updated version
    sleep(delay);
    FILE *usage_updated = fopen("/proc/stat", "r");

    fgets(temp, 1000, usage_updated);

    for (int i = 0; i < 1000; i++)
    {
        if (!isdigit(temp[i]) && temp[i] != '\0')
        { // Replace all non digit characters with whitespace so we can use scanf
            temp[i] = ' ';
        }
    }

    sscanf(temp, " %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", &user_time, &nice_time, &system_time, &idle_time, &iowait_time, &irq_time, &softirq_time, &steal_time, &guest_time, &guest_nice_time);

    total_time2 = user_time + nice_time + system_time + idle_time + iowait_time + irq_time + softirq_time + steal_time;
    total_idle_time2 = idle_time + iowait_time;
    total_usage_time2 = total_time2 - total_idle_time2;

    long long total_usage_dif = total_usage_time2 - total_usage_time;
    long long total_time_dif = total_time2 - total_time;

    float cpu_usage = (float)total_usage_dif / total_time_dif; // Same cpu usage formula as explained above
    cpu_usage *= 100;

    fclose(usage_updated);
    fclose(coreinfo);

    return cpu_usage;
}

void userInfo(int delay)
{

    setutent(); // setutent() opens the utmp.h file, getutent() reads a line of the utmp.h file and stores the data represented from the line in the struct pointed to by user_data
    struct utmp *user_data;
    printf("---------------------------------------\n\n");
    printf("### Sessions/users ###\n");

    while ((user_data = getutent()) != NULL)
    { // If there are no more lines left in the utmp file, end the loop
        if (user_data->ut_type == USER_PROCESS)
        { // If the line type is a user entry, print desired information about it
            printf("%s    %s    %s\n", user_data->ut_user, user_data->ut_line, user_data->ut_host);
        }
    }
    printf("---------------------------------------\n");
    endutent(); // Close the file pointer
    sleep(delay);
}

void start(float cpu_usage, int delay, int samples, int sys_flag, int user_flag, int sequential_flag)
{
    char storage[samples][100]; // Array of strings, each element is an entry of memory usage
    int k = 0;                  // K is the index of storage that we will store the current memory entry in
    struct sysinfo ram_info;

    for (int i = 0; i < samples; i++)
    { // Print sample amount of samples, if the sequential_flag is off, use escape key to refresh the screen
        if (sequential_flag == 0)
        {
            printf("\033c");
        }
        if(user_flag == 0){
            printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

        }
        

        if (sequential_flag == 0 && user_flag == 0)
        {
            for (int j = 0; j < i; j++)
            { // Print all of the previously stored memory usages
                printf("%s", storage[j]);
                k = j + 1;
            }
        }

        sysinfo(&ram_info); // Store ram info in the struct

        float total_physical = (9.31 * pow(10, -10)) * (ram_info.totalram / ram_info.mem_unit); // the 9.31 * pow(10,-10) converts it to GB
        float current_usage = (9.31 * pow(10, -10)) * (ram_info.totalram - ram_info.freeram);
        float total_virtual = (9.31 * pow(10, -10)) * ((ram_info.totalram + ram_info.totalswap) / ram_info.mem_unit);

        if(user_flag == 0){
            printf("%.2fGB / %.2fGB  --  %.2fGB / %.2fGB\n", current_usage, total_physical, current_usage, total_virtual);

        }
        

        if (sequential_flag == 0 && user_flag == 0)
        {
            for (int j = samples - 1 - i; j > 0; j--)
            { // Print enough whitespace to hold sample samples
                printf("\n");
            }
        }

        if (sys_flag == 0 && user_flag == 0)
        {
            userInfo(0);
            float current = cpuInfo(cpu_usage, delay);
            cpu_usage = current;
            sysInfo();
        }
        else if (sys_flag == 1 && user_flag == 0)
        { // Different cases depending on --user and --system arguments
            float current = cpuInfo(cpu_usage, delay);
            cpu_usage = current;
            sysInfo();
        }
        else if (sys_flag == 0 && user_flag == 1)
        {
            userInfo(delay);
            sysInfo();
        }
        else
        {
            printf("Invalid combination of flags! Exiting program.\n");
            exit(-1);
        }
        // If not in sequential mode, store the newly created memory usage formatted string in the array of strings
        if (sequential_flag == 0 && user_flag == 0)
        {
            sprintf(storage[k], "%.2fGB / %.2fGB  --  %.2fGB / %.2fGB\n", current_usage, total_physical, current_usage, total_virtual);
        }
    }
}

int main(int argc, char **argv)
{

    float cpu_usage = quick_cpu();
    int delay, samples, sys_flag, user_flag, sequential_flag;
    delay = 1;
    samples = 10;
    sys_flag = 0; // Set default values
    user_flag = 0;
    sequential_flag = 0;
    char *temp;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--sequential") == 0)
        {
            sequential_flag = 1;
        }
        if (strcmp(argv[i], "--user") == 0)
        {
            user_flag = 1;
        }
        if (strcmp(argv[i], "--system") == 0)
        { // iterate through the command line arguments and update flags accordingly
            sys_flag = 1;
        }
        if (strstr(argv[i], "--tdelay=") != NULL)
        {
            temp = &argv[i][0] + 9;
            delay = (int)strtol(temp, NULL, 10); // strstr(str a, str b) returns something other than null iff b is a substring of a
        }
        if (strstr(argv[i], "--samples=") != NULL)
        {
            temp = &argv[i][0] + 10;
            samples = (int)strtol(temp, NULL, 10);
        }
    }

    start(cpu_usage, delay, samples, sys_flag, user_flag, sequential_flag);

    return 0;
}
