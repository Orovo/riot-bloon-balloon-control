#include "dataAccess.h"

struct access_data total_data_access = {0};

unsigned int last_access_time = 0;
unsigned int data_refresh_rate_ms = 0;

kernel_pid_t data_access_thread = NULL;

static char _data_access_stack[THREAD_STACKSIZE_DEFAULT];

static mutex_t accessData_lock = MUTEX_INIT;
static mutex_t refreshData_lock = MUTEX_INIT;
static mutex_t dataAccess_thread_lock = MUTEX_INIT_LOCKED;
static mutex_t dataAccess_accessor_lock = MUTEX_INIT_LOCKED;

enum DATATYPE {
    unidentified,
    all_data, 
        gps_data,
        all_atmospheric_data,
            humidity, temperature, pressure
};

static enum DATATYPE dataIdentifier = unidentified;

void *dataAccessThread(void *arg) {
    saul_reg_t *devHum = saul_reg_find_type(SAUL_SENSE_HUM);
    saul_reg_t *devTemp = saul_reg_find_type(SAUL_SENSE_TEMP);
    saul_reg_t *devPres = saul_reg_find_type(SAUL_SENSE_PRESS);
    while(true) {
        //selflock until called
        mutex_lock(&dataAccess_thread_lock);

        //gather data - choose data to extract
        switch(dataIdentifier) {
            case all_data:
            case gps_data:
                getGPSData(&total_data_access.gps);
                if(dataIdentifier == gps_data) break;
            case all_atmospheric_data:
            case humidity:
                saul_reg_read(devHum, &total_data_access.atmospheric.humidity);
                if(dataIdentifier == humidity) break;
            case temperature:
                saul_reg_read(devTemp, &total_data_access.atmospheric.temperature);
                if(dataIdentifier == temperature) break;
            case pressure:
                saul_reg_read(devPres, &total_data_access.atmospheric.pressure);
                break;
            default:
                printf("Error in dataAccessThread: unexpedted value in dataIdentifier (%i)", dataIdentifier);
                break;
        }

        // reset dataIdentifier
        dataIdentifier = unidentified;
        
        mutex_unlock(&dataAccess_accessor_lock);
    }
}

void initializeDataAccess(unsigned int microseconds) {
    data_refresh_rate_ms = microseconds;
    data_access_thread = thread_create(_data_access_stack, sizeof(_data_access_stack), THREAD_PRIORITY_MAIN - 1, 0, dataAccessThread, NULL, "Data_Access Thread");
    initGPSData(data_access_thread);
}

void refreshData(void) {
    mutex_lock(&refreshData_lock);
    unsigned int current_time = xtimer_now_usec();
    if(last_access_time == 0 || (current_time >= (last_access_time + data_refresh_rate_ms))) {
        //not set or to old - refresh
        last_access_time = current_time;
        mutex_unlock(&dataAccess_thread_lock);
        mutex_lock(&dataAccess_accessor_lock); //pausing thread until dataAccesThread has collected all the data
    }
    mutex_unlock(&refreshData_lock);
}

int accessGPSData(struct gps_data *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = gps_data;
    refreshData();
    memcpy(other, &total_data_access.gps, sizeof(struct gps_data));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}

int accessAtmosphericData(struct atmospheric_data *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = all_atmospheric_data;
    refreshData();
    memcpy(other, &total_data_access.atmospheric, sizeof(struct atmospheric_data));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}

int accessTotalData(struct access_data *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = all_data;
    refreshData();
    memcpy(other, &total_data_access, sizeof(struct access_data));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}

int accessHumidityData(phydat_t *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = humidity;
    refreshData();
    memcpy(other, &total_data_access.atmospheric.humidity, sizeof(phydat_t));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}

int accessTemperatureData(phydat_t *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = temperature;
    refreshData();
    memcpy(other, &total_data_access.atmospheric.temperature, sizeof(phydat_t));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}

int accessPressureData(phydat_t *other) {
    if(other == NULL) return 1; //not fine
    mutex_lock(&accessData_lock);
    dataIdentifier = pressure;
    refreshData();
    memcpy(other, &total_data_access.atmospheric.pressure, sizeof(phydat_t));
    mutex_unlock(&accessData_lock);
    return 0; //everything fine
}