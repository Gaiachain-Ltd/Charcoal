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
    const QLatin1String types = QLatin1String("types");
    const QLatin1String cooperativeId = QLatin1String("cooperative_id");
    const QLatin1String lastAction = QLatin1String("last_action");
    const QLatin1String results = QLatin1String("results");
    const QLatin1String packageType = QLatin1String("package_type");
    const QLatin1String action = QLatin1String("action");
    const QLatin1String location = QLatin1String("location");
    const QLatin1String latitude = QLatin1String("latitude");
    const QLatin1String longitude = QLatin1String("longitude");
    const QLatin1String timestamp = QLatin1String("timestamp");
    const QLatin1String properties = QLatin1String("properties");
    const QLatin1String relations = QLatin1String("relations");
    const QLatin1String qrCode = QLatin1String("qr_code");
    const QLatin1String user = QLatin1String("user");
    const QLatin1String producers = QLatin1String("producers");
    const QLatin1String companies = QLatin1String("companies");
    const QLatin1String destinations = QLatin1String("destinations");
    const QLatin1String id = QLatin1String("id");
    const QLatin1String parentId = QLatin1String("parent_id");
    const QLatin1String name = QLatin1String("name");
    const QLatin1String type = QLatin1String("type");
    const QLatin1String village = QLatin1String("village");
    const QLatin1String parcels = QLatin1String("parcels");
    const QLatin1String parcel = QLatin1String("parcel");
    // Charcoal
    const QLatin1String ovenTypes = QLatin1String("oven-types");
    const QLatin1String treeSpecies = QLatin1String("tree-species");
    const QLatin1String villages = QLatin1String("villages");
    const QLatin1String plot = QLatin1String("plot");
    const QLatin1String entity = QLatin1String("entity");
    const QLatin1String typeId = QLatin1String("typeId");
    const QLatin1String entityId = QLatin1String("entityId");
    const QLatin1String date = QLatin1String("date");
    const QLatin1String eventDate = QLatin1String("eventDate");
    const QLatin1String actionName = QLatin1String("actionName");
    const QLatin1String parent = QLatin1String("parent");
    const QLatin1String bags = QLatin1String("bags");
    const QLatin1String active = QLatin1String("active");
    const QLatin1String locationLatitude = QLatin1String("locationLatitude");
    const QLatin1String locationLongitude = QLatin1String("locationLongitude");
    // Charcoal user data
    const QLatin1String code = QLatin1String("code");
    const QLatin1String contact = QLatin1String("contact");
    const QLatin1String function = QLatin1String("function");
    const QLatin1String fullName = QLatin1String("full_name");
    // Charcoal user roles
    const QLatin1String superUser = QLatin1String("SUPER_USER");
    const QLatin1String director = QLatin1String("DIRECTOR");
    const QLatin1String carbonizer = QLatin1String("CARBONIZER");
    const QLatin1String logger = QLatin1String("LOGGER");
    // Charcoal Event reply fields and charcoal Replantation reply fields
    const QLatin1String eventTimestamp = QLatin1String("timestamp");
    const QLatin1String webId = QLatin1String("webId");
    const QLatin1String webEventId = QLatin1String("package_id");
    const QLatin1String webPlotId = QLatin1String("plot_id");
    const QLatin1String webHarvestId = QLatin1String("harvest_id");
    const QLatin1String webNumberOfTrees = QLatin1String("number_of_trees");
    const QLatin1String webTreeSpecies = QLatin1String("tree_specie");
    const QLatin1String webOvenId = QLatin1String("oven_id");
    const QLatin1String webOvenType = QLatin1String("oven_type");
    const QLatin1String webOvenHeight = QLatin1String("oven_height");
    const QLatin1String webOvenHeight2 = QLatin1String("oven_height2");
    const QLatin1String webOvenWidth = QLatin1String("oven_width");
    const QLatin1String webOvenLength = QLatin1String("oven_length");
    const QLatin1String webOvenVolume = QLatin1String("oven_volume");
    const QLatin1String webBeginningDate = QLatin1String("beginning_date");
    const QLatin1String webEndingDate = QLatin1String("ending_date");
    const QLatin1String webEventDate = QLatin1String("event_date");
    const QLatin1String webQrCodes = QLatin1String("bags_qr_codes");
    const QLatin1String webPlateNumber = QLatin1String("plate_number");
    const QLatin1String webDestination = QLatin1String("destination");
    const QLatin1String webDocuments = QLatin1String("documents_photos");
    const QLatin1String webReceipts = QLatin1String("receipt_photos");
    const QLatin1String webParcel = QLatin1String("parcel");
    const QLatin1String webVillage = QLatin1String("village");
    const QLatin1String documents = QLatin1String("documents");
    const QLatin1String receipts = QLatin1String("receipts");
    const QLatin1String document = QLatin1String("document");
    const QLatin1String receipt = QLatin1String("receipt");
};

namespace StaticValues {
    const QLatin1String separator = QLatin1String(",");
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
