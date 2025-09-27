import json
import pandas as pd
import numpy as np
from Get_Data_json_2 import process_json_data
from stringer_cost import (
    tman_CH_guangyan, tman_CH_guhua,
    Part_Labor_Cost_CH
)
from skin_cost import (
    tman_skin_guangyan,
    Part_Labor_Cost_skin
)

process_json_data()

# 判断 value 是否在目标列表中
def check_contains(value, target_list):
    return value in target_list

# 辅助函数：加载 JSON
def load_json(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        return json.load(f)

# ✅ 最终主函数：从输入 JSON 的字段名判断长桁/蒙皮，然后计算并保存 JSON + CSV
def calculate_json_data(
    input_json_path,
    output_json_path,
    output_csv_path
):
    # --------------------------------------------
    # Step 0：加载输入 JSON 数据
    # --------------------------------------------
    input_data = load_json(input_json_path)

    if not isinstance(input_data, list):
        raise ValueError("输入 JSON 必须是一个列表，每项为一个工艺步骤（字典）")

    if len(input_data) == 0:
        raise ValueError("输入 JSON 数据不能为空")

    results = []

    # --------------------------------------------
    # Step 1：遍历每一行（每个工艺步骤字典）
    # --------------------------------------------
    for row in input_data:
        if not isinstance(row, dict):
            raise ValueError("每行数据必须是一个字典")

        component_type = None
        process_method = None
        chi = None
        num_person = None
        setup = None
        nu = None
        labor_rate = None
        machine_rate = None

        # =============================
        # ✅ 关键修改部分开始（只改这里！）============================
        # -------------------------------------------------------------
        # 通过遍历字段名，找到 Part1 ~ Part19，根据数字判断长桁/蒙皮
        # -------------------------------------------------------------
        component_type = None
        for key in row.keys():
            if key.startswith("Part"):
                try:
                    # 提取 Part 后的数字，如 Part1 -> 1, Part12 -> 12
                    part_num = int(key[4:])  # 去掉 'Part' 前缀
                except ValueError:
                    continue  # 不是数字，跳过该字段

                # 判断是长桁还是蒙皮
                if 1 <= part_num <= 6:
                    component_type = "长桁"
                    break
                elif 7 <= part_num <= 10:
                    component_type = "蒙皮"
                    break

        # 如果未找到任何 Part字段，报错
        if component_type is None:
            raise ValueError("未找到有效的 Part字段（如 Part1~Part19），无法判断是长桁还是蒙皮")

        # -------------------------------------------------------------
        # 工艺方法（如 "自动下料"）仍然取自 "Part1" 的 value（保持你原有逻辑）
        # 假定 "Part1" 的 value 就是工艺方法，如 "自动下料"、"固化" 等
        process_method = next((value for key, value in row.items() if "part" in key.lower()), None)
        # ============================= 关键修改部分结束 =============================

        # 其它字段保持不变，和你原来一样
        for key, value in row.items():
            if key == "特征尺寸（kg\mm\m2）":
                chi = float(value) if value is not None else 0.0
            elif key == "人数":
                num_person = int(value) if value is not None else 1
            elif key == "准备时间（min）":
                setup = float(value) if value is not None else 0.0
            elif key == "速率（kg/min、mm/min、m2/min）":
                nu = float(value) if value is not None else 0.0
            elif key == "设备加工时间（min）":
                machine_hours = float(value) if value is not None else 0.0
            elif key == "人工工时（min）":
                man_hours = float(value) if value is not None else 0.0
            elif key == "人工费率（元/h）":
                labor_rate = float(value) if value is not None else 0.0
            elif key == "设备费率（元/h）":
                machine_rate = float(value) if value is not None else 0.0
            elif key == "工时费（元）":
                part_labor_cost = float(value) if value is not None else 0.0

        if process_method is None:
            raise ValueError("未找到字段 'Part1' 的工艺方法（value），如 '自动下料'")

        print(f"✅ 处理工艺步骤：{process_method}，构件类型：{component_type}")

        # Step 2：根据构件类型和工艺方法，进行计算（保持你原来的逻辑不变）
        if process_method in ["A", "D", "J", "M", "R"]:
            if component_type == "长桁":
                machine_hours = tman_CH_guangyan(setup, 0.0, chi, nu)
                man_hours = machine_hours * num_person
                part_labor_cost = Part_Labor_Cost_CH(man_hours, labor_rate, machine_hours, machine_rate)
            elif component_type == "蒙皮":
                machine_hours = tman_skin_guangyan(setup, 0.0, chi, nu)
                man_hours = machine_hours * num_person
                part_labor_cost = Part_Labor_Cost_skin(man_hours, labor_rate, machine_hours, machine_rate)
            else:
                raise ValueError(f"未知构件类型：{component_type}")
        elif process_method == "G":
            if component_type == "长桁":
                machine_hours = tman_CH_guhua(setup, 0.0, chi, nu)
                man_hours = machine_hours * num_person
                part_labor_cost = Part_Labor_Cost_CH(man_hours, labor_rate, machine_hours, machine_rate)
            else:
                raise ValueError(f"蒙皮暂不支持固化工艺，当前工艺：{process_method}")
        else:
            if component_type == "长桁":
                machine_hours = tman_CH_guangyan(setup, 0.0, chi, nu)
                man_hours = machine_hours * num_person
                part_labor_cost = Part_Labor_Cost_CH(man_hours, labor_rate, machine_hours, machine_rate)
            elif component_type == "蒙皮":
                machine_hours = tman_skin_guangyan(setup, 0.0, chi, nu)
                man_hours = machine_hours * num_person
                part_labor_cost = Part_Labor_Cost_skin(man_hours, labor_rate, machine_hours, machine_rate)
            else:
                raise ValueError(f"未知构件类型：{component_type}")

        # Step 3：保存结果（保持原样）
        calculated_row = row.copy()
        calculated_row["设备加工时间（min）"] = round(machine_hours, 3)
        calculated_row["人工工时（min）"] = round(man_hours, 3)
        calculated_row["工时费（元）"] = round(part_labor_cost, 3)

        results.append(calculated_row)

    # Step 4 & 5：保存 JSON 和 CSV（保持原样）
    with open(output_json_path, "w", encoding="utf-8") as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    print(f"✅ JSON 结果已保存至：{output_json_path}")

    df = pd.DataFrame(results)
    df.to_csv(output_csv_path, index=False, encoding="utf-8-sig")
    print(f"✅ CSV 结果已保存至：{output_csv_path}")

# 执行主函数（保持原样）
if __name__ == "__main__":
    calculate_json_data(
        input_json_path="Enhanced_Result.json",
        output_json_path="calculated_result.json",
        output_csv_path="calculated_result.csv"
    )