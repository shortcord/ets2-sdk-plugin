#ifndef SCS_CFG_HANDLERS_H
#define SCS_CFG_HANDLERS_H

// SDK
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"

typedef struct scsConfigHandler_s
{
	const char* id;
	void (*handle) (const scs_named_value_t* current);
} scsConfigHandler_t;

#define scsConfigHandle(n) void handle##n (const scs_named_value_t* current)

// Define prototypes for all the various handlers

extern const char* current_config_id;

// Truck info etc.
scsConfigHandle(Id);
scsConfigHandle(CargoId);

scsConfigHandle(TruckMake);
scsConfigHandle(TruckMakeId);
scsConfigHandle(TruckModel);
scsConfigHandle(LicensePlate);

// Technical vehicle info
scsConfigHandle(FuelCapacity);
scsConfigHandle(FuelWarningFactor);
scsConfigHandle(AdblueCapacity);
scsConfigHandle(AirPressureWarning);
scsConfigHandle(AirPressureEmergency);
scsConfigHandle(OilPressureWarning);
scsConfigHandle(WaterTemperatureWarning);
scsConfigHandle(BatteryVoltageWarning);
scsConfigHandle(RpmLimit);
scsConfigHandle(FGearCount);
scsConfigHandle(RGearCount);
scsConfigHandle(RetarderStepCount);
scsConfigHandle(CabinPosition);
scsConfigHandle(HeadPosition);
scsConfigHandle(HookPosition);
scsConfigHandle(ShifterType);

// Job info
scsConfigHandle(JobIncome);
scsConfigHandle(JobDeadline);
scsConfigHandle(PlannedDistance);
scsConfigHandle(CargoLoaded);
scsConfigHandle(TrailerMass);
scsConfigHandle(TrailerId);
scsConfigHandle(TrailerName);
scsConfigHandle(CitySrc);
scsConfigHandle(CityDst);
scsConfigHandle(CompSrc);
scsConfigHandle(CompDst);

bool handleCfg(const scs_named_value_t* info);

#endif
