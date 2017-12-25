#ifndef FIGURE_ACTION_H
#define FIGURE_ACTION_H

enum {
    FIGURE_ACTION_1_IMMIGRANT_CREATED = 1,
    FIGURE_ACTION_2_IMMIGRANT_ARRIVING = 2,
    FIGURE_ACTION_3_IMMIGRANT_ENTERING_HOUSE = 3,
    FIGURE_ACTION_4_EMIGRANT_CREATED = 4,
    FIGURE_ACTION_5_EMIGRANT_EXITING_HOUSE = 5,
    FIGURE_ACTION_6_EMIGRANT_LEAVING = 6,
    FIGURE_ACTION_7_HOMELESS_CREATED = 7,
    FIGURE_ACTION_8_HOMELESS_GOING_TO_HOUSE = 8,
    FIGURE_ACTION_9_HOMELESS_ENTERING_HOUSE = 9,
    FIGURE_ACTION_10_HOMELESS_LEAVING = 10,
    FIGURE_ACTION_20_CARTPUSHER_INITIAL = 20,
    FIGURE_ACTION_21_CARTPUSHER_DELIVERING_TO_WAREHOUSE = 21,
    FIGURE_ACTION_22_CARTPUSHER_DELIVERING_TO_GRANARY = 22,
    FIGURE_ACTION_23_CARTPUSHER_DELIVERING_TO_WORKSHOP = 23,
    FIGURE_ACTION_24_CARTPUSHER_AT_WAREHOUSE = 24,
    FIGURE_ACTION_25_CARTPUSHER_AT_GRANARY = 25,
    FIGURE_ACTION_26_CARTPUSHER_AT_WORKSHOP = 26,
    FIGURE_ACTION_27_CARTPUSHER_RETURNING = 27,
    FIGURE_ACTION_40_TAX_COLLECTOR_CREATED = 40,
    FIGURE_ACTION_41_TAX_COLLECTOR_ENTERING_EXITING = 41,
    FIGURE_ACTION_42_TAX_COLLECTOR_ROAMING = 42,
    FIGURE_ACTION_43_TAX_COLLECTOR_RETURNING = 43,
    FIGURE_ACTION_50_WAREHOUSEMAN_CREATED = 50,
    FIGURE_ACTION_51_WAREHOUSEMAN_DELIVERING_RESOURCE = 51,
    FIGURE_ACTION_52_WAREHOUSEMAN_AT_DELIVERY_BUILDING = 52,
    FIGURE_ACTION_53_WAREHOUSEMAN_RETURNING_EMPTY = 53,
    FIGURE_ACTION_54_WAREHOUSEMAN_GETTING_FOOD = 54,
    FIGURE_ACTION_55_WAREHOUSEMAN_AT_GRANARY = 55,
    FIGURE_ACTION_56_WAREHOUSEMAN_RETURNING_WITH_FOOD = 56,
    FIGURE_ACTION_57_WAREHOUSEMAN_GETTING_RESOURCE = 57,
    FIGURE_ACTION_58_WAREHOUSEMAN_AT_WAREHOUSE = 58,
    FIGURE_ACTION_59_WAREHOUSEMAN_RETURNING_WITH_RESOURCE = 59,
    FIGURE_ACTION_60_ENGINEER_CREATED = 60,
    FIGURE_ACTION_61_ENGINEER_ENTERING_EXITING = 61,
    FIGURE_ACTION_62_ENGINEER_ROAMING = 62,
    FIGURE_ACTION_63_ENGINEER_RETURNING = 63,
    FIGURE_ACTION_70_PREFECT_CREATED = 70,
    FIGURE_ACTION_71_PREFECT_ENTERING_EXITING = 71,
    FIGURE_ACTION_72_PREFECT_ROAMING = 72,
    FIGURE_ACTION_73_PREFECT_RETURNING = 73,
    FIGURE_ACTION_74_PREFECT_GOING_TO_FIRE = 74,
    FIGURE_ACTION_75_PREFECT_AT_FIRE = 75,
    FIGURE_ACTION_76_PREFECT_GOING_TO_ENEMY = 76,
    FIGURE_ACTION_77_PREFECT_AT_ENEMY = 77,
    FigureActionState_80_SoldierAtRest = 80,
    FigureActionState_81_SoldierGoingToFort = 81,
    FigureActionState_82_SoldierReturningToBarracks = 82,
    FigureActionState_83_SoldierGoingToStandard = 83,
    FigureActionState_84_SoldierAtStandard = 84,
    FigureActionState_85_SoldierGoingToMilitaryAcademy = 85,
    FigureActionState_86_SoldierMoppingUp = 86,
    FigureActionState_87_SoldierGoingToDistantBattle = 87,
    FigureActionState_88_SoldierReturningFromDistantBattle = 88,
    FigureActionState_89_SoldierAtDistantBattle = 89,
    FIGURE_ACTION_90_ENTERTAINER_AT_SCHOOL_CREATED = 90,
    FIGURE_ACTION_91_ENTERTAINER_EXITING_SCHOOL = 91,
    FIGURE_ACTION_92_ENTERTAINER_GOING_TO_VENUE = 92,
    FIGURE_ACTION_94_ENTERTAINER_ROAMING = 94,
    FIGURE_ACTION_95_ENTERTAINER_RETURNING = 95,
    FigureActionState_100_TradeCaravanCreated = 100,
    FigureActionState_101_TradeCaravanArriving = 101,
    FigureActionState_102_TradeCaravanTrading = 102,
    FigureActionState_103_TradeCaravanLeaving = 103,
    FigureActionState_110_TradeShipCreated = 110,
    FigureActionState_111_TradeShipGoingToDock = 111,
    FigureActionState_112_TradeShipMoored = 112,
    FigureActionState_113_TradeShipGoingToDockQueue = 113,
    FigureActionState_114_TradeShipAnchored = 114,
    FigureActionState_115_TradeShipLeaving = 115,
    FigureActionState_120_RioterCreated = 120,
    FigureActionState_121_RioterMoving = 121,
    FIGURE_ACTION_125_ROAMING = 125,
    FIGURE_ACTION_126_ROAMER_RETURNING = 126,
    FIGURE_ACTION_128_FLOTSAM_CREATED = 128,
    FIGURE_ACTION_129_FLOTSAM_FLOATING = 129,
    FIGURE_ACTION_130_FLOTSAM_OFF_MAP = 130,
    FigureActionState_132_DockerIdling = 132,
    FigureActionState_133_DockerImportQueue = 133,
    FigureActionState_134_DockerExportQueue = 134,
    FigureActionState_135_DockerImportGoingToWarehouse = 135,
    FigureActionState_136_DockerExportGoingToWarehouse = 136,
    FigureActionState_137_DockerExportReturning = 137,
    FigureActionState_138_DockerImportReturning = 138,
    FigureActionState_139_DockerImportAtWarehouse = 139,
    FigureActionState_140_DockerExportAtWarehouse = 140,
    FIGURE_ACTION_145_MARKET_BUYER_GOING_TO_STORAGE = 145,
    FIGURE_ACTION_146_MARKET_BUYER_RETURNING = 146,
    FigureActionState_148_Fleeing = 148,
    FigureActionState_149_Corpse = 149,
    FigureActionState_150_Attack = 150,
    FigureActionState_151_EnemyInitial = 151,
    FigureActionState_152_EnemyWaiting = 152,
    FigureActionState_153_EnemyMarching = 153,
    FigureActionState_154_EnemyFighting = 154,
    FIGURE_ACTION_156_NATIVE_GOING_TO_MEETING_CENTER = 156,
    FIGURE_ACTION_157_NATIVE_RETURNING_FROM_MEETING = 157,
    FIGURE_ACTION_158_NATIVE_CREATED = 158,
    FIGURE_ACTION_159_NATIVE_ATTACKING = 159,
    FigureActionState_160_NativeTraderGoingToWarehouse = 160,
    FigureActionState_161_NativeTraderReturning = 161,
    FigureActionState_162_NativeTraderCreated = 162,
    FigureActionState_163_NativeTraderAtWarehouse = 163,
    FIGURE_ACTION_170_TOWER_SENTRY_AT_REST = 170,
    FIGURE_ACTION_171_TOWER_SENTRY_PATROLLING = 171,
    FIGURE_ACTION_172_TOWER_SENTRY_FIRING = 172,
    FIGURE_ACTION_173_TOWER_SENTRY_RETURNING = 173,
    FIGURE_ACTION_174_TOWER_SENTRY_GOING_TO_TOWER = 174,
    FIGURE_ACTION_180_BALLISTA_CREATED = 180,
    FIGURE_ACTION_181_BALLISTA_FIRING = 181,
    FIGURE_ACTION_190_FISHING_BOAT_CREATED = 190,
    FIGURE_ACTION_191_FISHING_BOAT_GOING_TO_FISH = 191,
    FIGURE_ACTION_192_FISHING_BOAT_FISHING = 192,
    FIGURE_ACTION_193_FISHING_BOAT_GOING_TO_WHARF = 193,
    FIGURE_ACTION_194_FISHING_BOAT_AT_WHARF = 194,
    FIGURE_ACTION_195_FISHING_BOAT_RETURNING_WITH_FISH = 195,
    FIGURE_ACTION_196_HERD_ANIMAL_AT_REST = 196,
    FIGURE_ACTION_197_HERD_ANIMAL_MOVING = 197,
    FIGURE_ACTION_199_WOLF_ATTACKING = 199,
    FIGURE_ACTION_200_HIPPODROME_HORSE_CREATED = 200,
    FIGURE_ACTION_201_HIPPODROME_HORSE_RACING = 201,
    FIGURE_ACTION_202_HIPPODROME_HORSE_DONE = 202,
};


#endif // FIGURE_ACTION_H
