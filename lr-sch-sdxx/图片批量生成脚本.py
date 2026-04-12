#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
合成数据生成器 - 可调位置与大小版
"""

import os
import cv2
import numpy as np
import random
import argparse
from pathlib import Path


class SyntheticDataGenerator:
    def __init__(self, 
                 cartoon_dir,
                 background_dir,
                 output_dir,
                 reference_image=None,
                 num_samples=100,
                 seed=None,
                 screen_ratio=0.7,
                 safe_margin=0.08,
                 center_y_offset=0.05,  # 新增：垂直偏移（正值向下）
                 size_boost=1.0):         # 新增：尺寸放大系数
        self.cartoon_dir = Path(cartoon_dir)
        self.background_dir = Path(background_dir)
        self.output_dir = Path(output_dir)
        self.num_samples = num_samples
        self.screen_ratio = screen_ratio
        self.safe_margin = safe_margin
        self.center_y_offset = center_y_offset  # 例如0.05表示向下偏移5%屏幕高度
        self.size_boost = size_boost            # 例如1.15表示放大15%
        
        if seed is not None:
            random.seed(seed)
            np.random.seed(seed)
        
        self.images_dir = self.output_dir / "images"
        self.images_dir.mkdir(parents=True, exist_ok=True)
        
        # 加载参考图
        self.ref_img = None
        if reference_image and os.path.exists(reference_image):
            self.ref_img = cv2.imread(reference_image)
        
        self.cartoon_images = self._load_cartoons()
        self.background_images = self._load_backgrounds()
        
        # 计算参数
        if self.ref_img is not None:
            self.ref_brightness = self._compute_brightness(self.ref_img)
            self.area_ratio = self._compute_area_ratio_from_ref() * self.size_boost
        else:
            self.ref_brightness = 128.0
            self.area_ratio = 0.55 * self.size_boost
            
        print(f"✓ 加载了 {len(self.cartoon_images)} 张卡通图片")
        print(f"✓ 加载了 {len(self.background_images)} 张背景图片")
        print(f"✓ 目标亮度: {self.ref_brightness:.1f}")
        print(f"✓ 面积比例: {self.area_ratio:.1%} (放大系数: {self.size_boost})")
        print(f"✓ 垂直偏移: 向下 {self.center_y_offset:.1%}")
        
    def _remove_border(self, img, threshold=30):
        """去除图片边缘黑边"""
        if len(img.shape) == 3 and img.shape[2] == 4:
            mask = img[:, :, 3] > 50
        else:
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) if len(img.shape) == 3 else img
            mask = gray > threshold
        
        rows = np.any(mask, axis=1)
        cols = np.any(mask, axis=0)
        
        if not np.any(rows) or not np.any(cols):
            return img
        
        ymin, ymax = np.where(rows)[0][[0, -1]]
        xmin, xmax = np.where(cols)[0][[0, -1]]
        
        margin = 3
        h, w = img.shape[:2]
        ymin = max(0, ymin - margin)
        ymax = min(h, ymax + margin + 1)
        xmin = max(0, xmin - margin)
        xmax = min(w, xmax + margin + 1)
        
        return img[ymin:ymax, xmin:xmax]
    
    def _load_cartoons(self):
        cartoons = []
        valid_ext = {'.png', '.jpg', '.jpeg', '.webp'}
        
        for f in self.cartoon_dir.iterdir():
            if f.suffix.lower() in valid_ext:
                img = cv2.imread(str(f), cv2.IMREAD_UNCHANGED)
                if img is None:
                    continue
                
                if len(img.shape) == 2:
                    img = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
                elif img.shape[2] == 4:
                    alpha = img[:, :, 3:4] / 255.0
                    img_rgb = img[:, :, :3]
                    black_bg = np.zeros_like(img_rgb)
                    img = (img_rgb * alpha + black_bg * (1 - alpha)).astype(np.uint8)
                
                img = self._remove_border(img, threshold=30)
                
                if img.shape[0] > 20 and img.shape[1] > 20:
                    cartoons.append({'image': img, 'name': f.stem})
        return cartoons
    
    def _load_backgrounds(self):
        backgrounds = []
        valid_ext = {'.png', '.jpg', '.jpeg', '.webp', '.bmp'}
        
        for f in self.background_dir.iterdir():
            if f.suffix.lower() in valid_ext:
                img = cv2.imread(str(f), cv2.IMREAD_COLOR)
                if img is not None:
                    if len(img.shape) == 2:
                        img = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
                    backgrounds.append({'image': img, 'name': f.stem})
        return backgrounds
    
    def _compute_brightness(self, img):
        lab = cv2.cvtColor(img, cv2.COLOR_BGR2LAB)
        return float(np.mean(lab[:, :, 0]))
    
    def _compute_area_ratio_from_ref(self):
        """计算参考图中主体占屏幕的比例"""
        if self.ref_img is None:
            return 0.55
        
        h, w = self.ref_img.shape[:2]
        y_margin = int(h * 0.15)
        x_margin = int(w * 0.1)
        
        screen = self.ref_img[y_margin:h-y_margin, x_margin:w-x_margin]
        
        hsv = cv2.cvtColor(screen, cv2.COLOR_BGR2HSV)
        lower_blue = np.array([90, 50, 50])
        upper_blue = np.array([130, 255, 255])
        blue_mask = cv2.inRange(hsv, lower_blue, upper_blue)
        
        lower_black = np.array([0, 0, 0])
        upper_black = np.array([180, 255, 50])
        black_mask = cv2.inRange(hsv, lower_black, upper_black)
        
        bg_mask = cv2.bitwise_or(blue_mask, black_mask)
        char_mask = cv2.bitwise_not(bg_mask)
        
        kernel = np.ones((7,7), np.uint8)
        char_mask = cv2.morphologyEx(char_mask, cv2.MORPH_CLOSE, kernel)
        
        char_area = np.sum(char_mask > 0)
        screen_area = screen.shape[0] * screen.shape[1]
        
        ratio = char_area / screen_area if screen_area > 0 else 0.55
        return np.clip(ratio, 0.4, 0.75)
    
    def _get_screen_region(self, img):
        """获取屏幕区域"""
        h, w = img.shape[:2]
        y_margin = int(h * (1 - self.screen_ratio) / 2)
        x_margin = int(w * 0.1)
        
        x1 = x_margin
        y1 = y_margin
        sw = w - 2 * x_margin
        sh = h - 2 * y_margin
        
        return x1, y1, sw, sh
    
    def _match_brightness(self, src, target):
        """匹配亮度"""
        lab = cv2.cvtColor(src, cv2.COLOR_BGR2LAB).astype(np.float32)
        l = lab[:, :, 0]
        diff = target - np.mean(l)
        l = np.clip(l + diff, 0, 255)
        lab[:, :, 0] = l
        return cv2.cvtColor(lab.astype(np.uint8), cv2.COLOR_LAB2BGR)
    
    def _generate(self, background, cartoon, debug=False):
        """生成合成图 - 可调位置版本"""
        h, w = background.shape[:2]
        
        # 1. 获取屏幕区域
        sx, sy, sw, sh = self._get_screen_region(background)
        
        # 2. 计算内部安全区域
        inner_margin_y = int(sh * self.safe_margin)
        inner_margin_x = int(sw * self.safe_margin)
        
        usable_x = sx + inner_margin_x
        usable_y = sy + inner_margin_y
        usable_w = sw - 2 * inner_margin_x
        usable_h = sh - 2 * inner_margin_y
        
        # 3. 亮度匹配
        cartoon = self._match_brightness(cartoon, self.ref_brightness)
        background = self._match_brightness(background, self.ref_brightness)
        
        # 4. 计算卡通图尺寸（应用放大系数）
        ch, cw = cartoon.shape[:2]
        target_area = usable_w * usable_h * self.area_ratio
        scale = np.sqrt(target_area / (ch * cw))
        
        max_w = int(usable_w * 0.9)
        max_h = int(usable_h * 0.9)
        
        new_w = min(int(cw * scale), max_w)
        new_h = min(int(ch * scale), max_h)
        
        if new_w < 30 or new_h < 30:
            new_w = int(usable_w * 0.6)
            new_h = int(usable_h * 0.6)
        
        # 5. 缩放
        resized = cv2.resize(cartoon, (new_w, new_h), interpolation=cv2.INTER_LANCZOS4)
        
        # 6. 轻微旋转
        if random.random() > 0.4:
            angle = random.uniform(-3, 3)
            M = cv2.getRotationMatrix2D((new_w//2, new_h//2), angle, 1.0)
            rotated = cv2.warpAffine(resized, M, (new_w, new_h),
                                    borderMode=cv2.BORDER_CONSTANT,
                                    borderValue=(0,0,0))
            gray = cv2.cvtColor(rotated, cv2.COLOR_BGR2GRAY)
            _, mask = cv2.threshold(gray, 20, 255, cv2.THRESH_BINARY)
            coords = cv2.findNonZero(mask)
            if coords is not None:
                x, y, w_eff, h_eff = cv2.boundingRect(coords)
                if w_eff > 10 and h_eff > 10:
                    resized = rotated[y:y+h_eff, x:x+w_eff]
        
        # 7. 计算位置 - 关键修改：应用垂直偏移
        final_h, final_w = resized.shape[:2]
        cx = usable_x + usable_w // 2
        
        # 计算基础中心点（几何中心）
        base_cy = usable_y + usable_h // 2
        # 应用向下偏移（正值向下）
        offset_pixels = int(usable_h * self.center_y_offset)
        cy = base_cy + offset_pixels
        
        x1 = cx - final_w // 2
        y1 = cy - final_h // 2
        
        if debug:
            print(f"可用区域中心Y: {base_cy}, 偏移后: {cy} (向下{offset_pixels}px)")
        
        # 8. 边界检查（确保不超出屏幕）
        x1 = max(usable_x, min(x1, usable_x + usable_w - final_w))
        y1 = max(usable_y, min(y1, usable_y + usable_h - final_h))
        x2 = x1 + final_w
        y2 = y1 + final_h
        
        # 最终安全检查
        if x2 > usable_x + usable_w:
            x2 = usable_x + usable_w
            x1 = x2 - final_w
        if y2 > usable_y + usable_h:
            y2 = usable_y + usable_h
            y1 = y2 - final_h
            
        if debug:
            print(f"最终位置: ({x1},{y1})-({x2},{y2})")
        
        # 9. 合成
        result = background.copy()
        
        roi_h = y2 - y1
        roi_w = x2 - x1
        if resized.shape[0] != roi_h or resized.shape[1] != roi_w:
            resized = cv2.resize(resized, (roi_w, roi_h))
        
        result[y1:y2, x1:x2] = resized
        
        # 10. 阴影
        shadow = np.zeros((h, w, 3), dtype=np.uint8)
        shadow_cx = (x1 + x2) // 2
        shadow_cy = y2
        axes = (int(final_w * 0.5), int(final_h * 0.15))
        cv2.ellipse(shadow, (shadow_cx, shadow_cy), axes, 0, 0, 360, (30,30,30), -1)
        shadow = cv2.GaussianBlur(shadow, (25, 25), 0)
        result = cv2.addWeighted(result, 1.0, shadow, 0.3, 0)
        
        return result
    
    def generate_dataset(self, debug_first=False):
        print(f"\n开始生成 {self.num_samples} 个样本...")
        success = 0
        num_cartoons = len(self.cartoon_images)
        
        for i in range(self.num_samples):
            try:
                cartoon = self.cartoon_images[i % num_cartoons]
                bg = random.choice(self.background_images)
                
                result = self._generate(
                    bg['image'].copy(), 
                    cartoon['image'].copy(),
                    debug=(debug_first and i == 0)
                )
                
                path = self.images_dir / f"sample_{i:06d}.jpg"
                cv2.imwrite(str(path), result, [cv2.IMWRITE_JPEG_QUALITY, 95])
                success += 1
                
                if (i+1) % 100 == 0 or (i+1) == self.num_samples:
                    print(f"  已生成 {i+1}/{self.num_samples}")
                    
            except Exception as e:
                print(f"  样本 {i} 错误: {e}")
                continue
        
        print(f"\n✓ 完成! 成功生成 {success} 张图片")
        print(f"  输出目录: {self.images_dir}")


def main():
    parser = argparse.ArgumentParser(description='合成数据生成器 - 可调位置与大小版')
    parser.add_argument('-c', '--cartoon', required=True, help='卡通图片目录')
    parser.add_argument('-b', '--background', required=True, help='背景图片目录')
    parser.add_argument('-r', '--reference', required=True, help='参考图片路径')
    parser.add_argument('-o', '--output', default='./output', help='输出目录')
    parser.add_argument('-n', '--num', type=int, default=100, help='生成数量')
    parser.add_argument('-s', '--seed', type=int, default=42, help='随机种子')
    parser.add_argument('--margin', type=float, default=0.08, help='内部安全边距(默认0.08)')
    parser.add_argument('--down', type=float, default=0.05, 
                       help='向下偏移比例(默认0.05=5%%)，范围0.0-0.3')
    parser.add_argument('--scale', type=float, default=1.0, 
                       help='尺寸放大系数(默认1.0)，>1放大，<1缩小')
    parser.add_argument('--debug', action='store_true', help='调试模式')
    
    args = parser.parse_args()
    
    gen = SyntheticDataGenerator(
        cartoon_dir=args.cartoon,
        background_dir=args.background,
        output_dir=args.output,
        reference_image=args.reference,
        num_samples=args.num,
        seed=args.seed,
        safe_margin=args.margin,
        center_y_offset=args.down,
        size_boost=args.scale
    )
    
    gen.generate_dataset(debug_first=args.debug)


if __name__ == "__main__":
    main()