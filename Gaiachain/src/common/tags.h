#ifndef TAGS_H
#define TAGS_H

#include <QLatin1String>

namespace Tags {
    const QLatin1String authorizationHeader = QLatin1String("Authorization");
    const QLatin1String bearerHeader = QLatin1String("Bearer");
    const QLatin1String token = QLatin1String("token");
    const QLatin1String role = QLatin1String("role");
    const QLatin1String cooperativeId = QLatin1String("cooperative_id");
    const QLatin1String cooperativeName = QLatin1String("cooperative_name");
    const QLatin1String email = QLatin1String("email");
    const QLatin1String password = QLatin1String("password");
    const QLatin1String id = QLatin1String("id");
    const QLatin1String packageId = QLatin1String("pid");
    const QLatin1String packageIds = QLatin1String("pids");
    const QLatin1String timestampFrom = QLatin1String("from_timestamp");
    const QLatin1String timestampTo = QLatin1String("to_timestamp");
    const QLatin1String count = QLatin1String("count");
    const QLatin1String keyword = QLatin1String("keyword");
    const QLatin1String packageType = QLatin1String("package_type");
    const QLatin1String action = QLatin1String("action");
    const QLatin1String timestamp = QLatin1String("timestamp");
    const QLatin1String properties = QLatin1String("properties");
    const QLatin1String agent = QLatin1String("agent");
    const QLatin1String entities = QLatin1String("entities");
    const QLatin1String relations = QLatin1String("relations");
    const QLatin1String producers = QLatin1String("producers");
    const QLatin1String buyers = QLatin1String("buyers");
    const QLatin1String transporters = QLatin1String("transporters");
    const QLatin1String destinations = QLatin1String("destinations");
    const QLatin1String name = QLatin1String("name");
    const QLatin1String village = QLatin1String("village");
    const QLatin1String parcels = QLatin1String("parcels");
    const QLatin1String code = QLatin1String("code");
};

namespace StaticValues {
    const QLatin1String all = QLatin1String("ALL");

    const QLatin1String userSuperUser = QLatin1String("SUPER_USER");
    const QLatin1String userInspector = QLatin1String("INSPECTOR");
    const QLatin1String userPca = QLatin1String("PCA");
    const QLatin1String userWarehouseman = QLatin1String("WAREHOUSEMAN");
    const QLatin1String userCooperativeRepresentative = QLatin1String("COOPERATIVE_REPRESENTATIVE");

    const QLatin1String actionHarvest = QLatin1String("HARVEST");
    const QLatin1String actionGrainProcessing = QLatin1String("GRAIN_PROCESSING");
    const QLatin1String actionSectionReception = QLatin1String("SECTION_RECEPTION");
    const QLatin1String actionBagging = QLatin1String("BAGGING");
    const QLatin1String actionLotCreation = QLatin1String("LOT_CREATION");
    const QLatin1String actionWarehouseTransport = QLatin1String("WAREHOUSE_TRANSPORT");
    const QLatin1String actionExportReception = QLatin1String("EXPORT_RECEPTION");

    const QLatin1String packageHarvest = QLatin1String("HARVEST");
    const QLatin1String packageSac = QLatin1String("SAC");
    const QLatin1String packageLot = QLatin1String("LOT");
}

#endif // TAGS_H
