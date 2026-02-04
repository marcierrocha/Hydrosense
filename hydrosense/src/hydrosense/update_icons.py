from pathlib import Path
from PIL import Image
import os

def update_icons():
    # Source image
    source_image_path = Path("src/hydrosense/resources/2.jpeg")
    if not source_image_path.exists():
        print(f"Error: Source image not found at {source_image_path}")
        return

    # Target directory root
    # Note: adjusting path based on where script is run (project root presumed)
    res_dir = Path("build/hydrosense/android/gradle/app/src/main/res")
    
    if not res_dir.exists():
        print(f"Error: Android resources directory not found at {res_dir}")
        print("Make sure you have run 'briefcase create android' or 'briefcase build android' first.")
        return

    print(f"Scanning {res_dir} for mipmap folders...")

    targets = [
        "ic_launcher.png",
        "ic_launcher_round.png",
        "ic_launcher_foreground.png",
        "splash.png"
    ]

    count = 0
    # Walk through all directories
    for root, dirs, files in os.walk(res_dir):
        for dir_name in dirs:
            if "mipmap" in dir_name:
                mipmap_dir = Path(root) / dir_name
                print(f"Processing {mipmap_dir}...")
                
                for target_file in targets:
                    target_path = mipmap_dir / target_file
                    if target_path.exists():
                        try:
                            # Open existing to get size
                            with Image.open(target_path) as target_img:
                                target_size = target_img.size
                            
                            # Open source and resize
                            with Image.open(source_image_path) as source_img:
                                if hasattr(Image, 'Resampling'):
                                    resample_method = Image.Resampling.LANCZOS
                                else:
                                    resample_method = Image.LANCZOS
                                
                                resized_img = source_img.resize(target_size, resample_method)
                                resized_img.save(target_path)
                                print(f"  [UPDATED] {target_file} (resized to {target_size})")
                                count += 1
                        except Exception as e:
                            print(f"  [ERROR] Failed to update {target_path}: {e}")

    print(f"\nCompleted. Updated {count} files.")

if __name__ == "__main__":
    update_icons()
