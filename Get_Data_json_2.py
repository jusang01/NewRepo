import json
import pandas as pd
from typing import Dict, Any, List

def load_json(file_path: str) -> Any:
    """通用 JSON 文件加载函数"""
    with open(file_path, "r", encoding="utf-8") as f:
        return json.load(f)

def process_json_data():
    # === 1. 文件路径 ===
    main_json_path = "./长桁.json"          # 工艺步骤主表（文档2）
    manufacturing_json_path = "./制造速率.json"  # 含速率与准备时间（文档1）
    equipment_rate_json_path = "./设备费率.json"  # 含设备名称加型号 -> 设备费率（元/h）
    labor_rate_json_path = "./工时费率.json"     # 含人工费率，如 "复材制造": 80

    # === 2. 加载所有 JSON 数据 ===
    #main_data = load_json(main_json_path)
    main_data = load_json(main_json_path)["data"]
    manufacturing_data = load_json(manufacturing_json_path)
    equipment_rate_data = load_json(equipment_rate_json_path)
    labor_rate_data = load_json(labor_rate_json_path)

    # === 3. 构建 4 个映射字典 ===

    # 【1】制造速率.json → 人工速率名称 -> 速率（kg/min、mm/min、m2/min）
    manufacturing_rate_map: Dict[str, float] = {}
    manufacturing_tezhengchicun_map: Dict[str, float] = {}
    manufacturing_renshu_map: Dict[str, int] = {}
    manufacturing_prep_time_map: Dict[str, int] = {}  # 新增：人工速率名称 -> 准备时间（min）

    for item in manufacturing_data:
        rate_name = item.get("人工速率名称")
        tezheng_chixun = item.get("特征尺寸")
        ren_shu = item.get("备注")
        rate = item.get("速率（kg/min、mm/min、m2/min）")
        prep_time = item.get("准备时间（min）")

        if rate_name is not None:
            if tezheng_chixun is not None:
                try:
                    manufacturing_tezhengchicun_map[rate_name] = float(tezheng_chixun)
                except (TypeError, ValueError):
                    manufacturing_tezhengchicun_map[rate_name] = None
            if rate is not None:
                try:
                    manufacturing_rate_map[rate_name] = float(rate)
                except (TypeError, ValueError):
                    manufacturing_rate_map[rate_name] = None
            if ren_shu is not None:
                try:
                    manufacturing_renshu_map[rate_name] = int(ren_shu)
                except (TypeError, ValueError):
                    manufacturing_renshu_map[rate_name] = None
            if prep_time is not None:
                try:
                    manufacturing_prep_time_map[rate_name] = int(prep_time)
                except (TypeError, ValueError):
                    manufacturing_prep_time_map[rate_name] = None

    # 【2】设备费率.json → 设备名称加型号 -> 设备费率（元/h）
    equipment_rate_map: Dict[str, float] = {}
    for item in equipment_rate_data:
        device_name_key = item.get("设备名称加型号")  # 如 "A_1"
        rate = item.get("设备费率（元/h）")
        if device_name_key is not None and rate is not None:
            try:
                equipment_rate_map[device_name_key] = float(rate)
            except (TypeError, ValueError):
                equipment_rate_map[device_name_key] = None

    # # 【3】工时费率.json → 获取 "复材制造" 对应的人工费率（元/h）
    # labor_rate_composite = 0.0  # 默认值
    # if isinstance(labor_rate_data, dict):
    #     labor_rate_composite = labor_rate_data.get("复材制造", 0.0)
    # elif isinstance(labor_rate_data, list) and len(labor_rate_data) > 0:
    #     labor_rate_composite = labor_rate_data[0].get("复材制造", 0.0)
    # else:
    #     labor_rate_composite = 0.0
    # 判断是否是列表，然后遍历查找 "分类": "复材制造"
    if isinstance(labor_rate_data, list):
        for entry in labor_rate_data:
            if isinstance(entry, dict):
                分类 = entry.get("分类")
                人工费率 = entry.get("人工费率（元/h）")
                if 分类 == "复材制造" :
                    labor_rate_composite = 人工费率  # 提取到的值就是 180
                    break  # 找到后就退出循环



    # 其它情况（如 JSON 格式不符合预期），labor_rate_composite 保持默认值 0.0

    # 如果 labor_rate_data 是 {"B2": 50} 这种旧格式，但您现在要的是 "复材制造"，请按需调整
    # 上面已优先查找 "复材制造" key

    # === 4. 遍历大客壁板.json中的每一行，补充 4 项信息 ===
    results = []

    for row in main_data:
        # 工艺步骤名称，如 "自动下料"，对应制造速率中的人工速率名称
        # 获取第一个以 "Part" 开头的字段的值（如 Part1、Part2、... 中的第一个）
        process_step = None
        for key in row.keys():
            if key.startswith("Part"):
                process_step = row.get(key)
                break  # 只取第一个匹配的 Part 字段
        # process_step = row.get("Part1")  # 这是工艺步骤名，如"自动下料"
        device_name = row.get("设备名称")  # 用于查设备费率，如 "A_1"

        # 从映射表中获取各项数据
        rate = manufacturing_rate_map.get(process_step)  # 速率（kg/min、mm/min、m2/min）
        tezheng_chixun = manufacturing_tezhengchicun_map.get(process_step)
        ren_shu= manufacturing_renshu_map.get(process_step)
        prep_time = manufacturing_prep_time_map.get(process_step)  # 准备时间（min）
        device_rate = equipment_rate_map.get(device_name)  # 设备费率（元/h）
        labor_rate = labor_rate_composite  # 人工费率（元/h），来自"复材制造"

        # 构造结果行
        result_row = row.copy()  # 保留原始数据
        result_row["特征尺寸（kg\mm\m2）"] = tezheng_chixun
        result_row["人数"] = ren_shu
        result_row["准备时间（min）"] = prep_time
        result_row["速率（kg/min、mm/min、m2/min）"] = rate
        result_row["设备费率（元/h）"] = device_rate
        result_row["人工费率（元/h）"] = labor_rate

        results.append(result_row)



    # === 5. 保存结果为 CSV 和 JSON ===
    df_result = pd.DataFrame(results)

    # 输出 CSV（推荐，可用 Excel 打开）
    csv_output_path = "Enhanced_Result.csv"
    df_result.to_csv(csv_output_path, index=False, encoding="utf-8-sig")
    print(f"✅ 增强结果已保存为 CSV 文件：{csv_output_path}")

    # 输出 JSON
    json_output_path = "Enhanced_Result.json"
    with open(json_output_path, "w", encoding="utf-8") as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    print(f"✅ 增强结果已保存为 JSON 文件：{json_output_path}")

# 执行主函数
if __name__ == "__main__":
    process_json_data()