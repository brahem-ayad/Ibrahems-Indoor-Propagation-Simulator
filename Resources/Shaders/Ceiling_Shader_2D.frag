#version 330

// Input from your vertex shader
in vec3 fragWorldPos;
in vec4 fragColor;

// Output color
out vec4 finalColor;

void main()
{
    // Hardcoded design colors
    vec4 hatchColor   = vec4(1.0, 0.0, 0.0, 1.0);  // Solid black lines
    vec4 bgColor      = vec4(1.0, 1.0, 1.0, 1.0);  // Base background (white/transparent-ready)

    // Adjust these based on world units
    float hatchSpacing   = 16.0; // Distance between parallel lines
    float hatchThickness = 1.5;  // Thickness of the lines

    // 1. First diagonal direction (Bottom-Left to Top-Right)
    float pattern1 = mod(fragWorldPos.x + fragWorldPos.y, hatchSpacing);
    float isHatch1 = step(pattern1, hatchThickness);

    // 2. Second diagonal direction (Top-Left to Bottom-Right)
    float pattern2 = mod(fragWorldPos.x - fragWorldPos.y, hatchSpacing);
    float isHatch2 = step(pattern2, hatchThickness);

    // Combine both patterns using a logical OR (max takes whichever line is present)
    float isCrossHatch = max(isHatch1, isHatch2);

    // Mix background and cross-hatch lines together
    vec4 innerColor = mix(bgColor, hatchColor, isCrossHatch);

    // Combine with Raylib's geometry tint (this keeps your red/pink selection tint working!)
    finalColor = innerColor * fragColor;
}
