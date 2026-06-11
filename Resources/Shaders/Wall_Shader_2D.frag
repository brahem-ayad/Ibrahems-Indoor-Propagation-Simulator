#version 330

// Input from our new vertex shader
in vec3 fragWorldPos;
in vec4 fragColor;

// Output color
out vec4 finalColor;

void main()
{
    // Hardcoded design colors
    vec4 hatchColor  = vec4(0.0, 0.0, 0.0, 1.0);  // Solid black
    vec4 bgColor     = vec4(1.0, 1.0, 1.0, 1.0);  // Light gray

    // Adjust these based on world units/pixels (e.g., matching your grid spacing)
    float hatchSpacing   = 12.0; // Distance between lines in world units
    float hatchThickness = 2.0;  // Width of lines in world units

    // Calculate diagonal lines using locked world coordinates
    float hatchPattern = mod(fragWorldPos.x + fragWorldPos.y, hatchSpacing);

    // Determine if the current fragment lands on a stripe
    float isHatch = step(hatchPattern, hatchThickness);

    // Mix background and hatch stripes together
    vec4 innerColor = mix(bgColor, hatchColor, isHatch);

    // Combine with Raylib's geometry tint
    finalColor = innerColor * fragColor;
}
