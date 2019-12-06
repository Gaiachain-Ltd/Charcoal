#ifndef TAGS_H
#define TAGS_H

#include <QLatin1String>

namespace Tags {
    const QLatin1String authorizationHeader = QLatin1String("Authorization");
    const QLatin1String bearerHeader = QLatin1String("Bearer");
    const QLatin1String accessToken = QLatin1String("access");
    const QLatin1String refreshToken = QLatin1String("refresh");
    const QLatin1String role = QLatin1String("role");
    const QLatin1String company = QLatin1String("company");
    const QLatin1String companyId = QLatin1String("pid");
    const QLatin1String login = QLatin1String("email");
    const QLatin1String password = QLatin1String("password");
    const QLatin1String pid = QLatin1String("pid");
    const QLatin1String pids = QLatin1String("pids");
    const QLatin1String timestampFrom = QLatin1String("from_timestamp");
    const QLatin1String timestampTo = QLatin1String("to_timestamp");
    const QLatin1String limit = QLatin1String("limit");
    const QLatin1String offset = QLatin1String("offset");
    const QLatin1String keyword = QLatin1String("keyword");
    const QLatin1String lastAction = QLatin1String("last_action");
    const QLatin1String results = QLatin1String("results");
    const QLatin1String packageType = QLatin1String("package_type");
    const QLatin1String action = QLatin1String("action");
    const QLatin1String timestamp = QLatin1String("timestamp");
    const QLatin1String properties = QLatin1String("properties");
    const QLatin1String relations = QLatin1String("relations");
    const QLatin1String qrCode = QLatin1String("qr_code");
    const QLatin1String user = QLatin1String("user");
    const QLatin1String producers = QLatin1String("producers");
    const QLatin1String companies = QLatin1String("companies");
    const QLatin1String destinations = QLatin1String("destinations");
    const QLatin1String id = QLatin1String("id");
    const QLatin1String name = QLatin1String("name");
    const QLatin1String type = QLatin1String("type");
    const QLatin1String village = QLatin1String("village");
    const QLatin1String parcels = QLatin1String("parcels");
    const QLatin1String parcel = QLatin1String("parcel");
};

namespace StaticValues {
    const QLatin1String all = QLatin1String("ALL");

    const QLatin1String userSuperUser = QLatin1String("SUPER_USER");
    const QLatin1String userInspector = QLatin1String("INSPECTOR");
    const QLatin1String userPca = QLatin1String("PCA");
    const QLatin1String userWarehouseman = QLatin1String("WAREHOUSEMAN");
    const QLatin1String userCooperativeRepresentative = QLatin1String("COOPERATIVE_REPRESENTATIVE");

    const QLatin1String companyCooperative = QLatin1String("Cooperative");
    const QLatin1String companyBuyer = QLatin1String("Buyer");
    const QLatin1String companyTransporter = QLatin1String("Transporter");

    const QLatin1String actionHarvest = QLatin1String("HA");
    const QLatin1String actionGrainProcessing = QLatin1String("BR");    // BR - Breaking
    const QLatin1String actionSectionReception = QLatin1String("HR");
    const QLatin1String actionBagging = QLatin1String("BG");
    const QLatin1String actionLotCreation = QLatin1String("CR");
    const QLatin1String actionWarehouseTransport = QLatin1String("TR");
    const QLatin1String actionExportReception = QLatin1String("LR");

    const QLatin1String packageHarvest = QLatin1String("HAR");
    const QLatin1String packageSac = QLatin1String("SAC");
    const QLatin1String packageLot = QLatin1String("LOT");
}

#endif // TAGS_H
