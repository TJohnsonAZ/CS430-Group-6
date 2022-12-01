function start(image) {

    // 0. Connect to a webgl canvas / javascript plumbing

    var canvas = document.getElementById("webgl");
    var gl = canvas.getContext("webgl");

    var scale = 1.0;
    var theta = 0;
    var x_translation = 0;
    var y_translation = 0;
    var x_shear = 0;

    document.addEventListener('keydown', (event) => {
        if (event.key == 's') {
            scale -= 0.1;
        }
        if (event.key == 'S') {
            scale += 0.1;
        }
        if (event.key == 'h') {
            x_shear -= 0.1;
        }
        if (event.key == 'H') {
            x_shear += 0.1;
        }
        if (event.key == 'r') {
            theta -= 0.1;
        }
        if (event.key == 'R') {
            theta += 0.1;
        }
        if (event.key == "ArrowLeft") {
            x_translation -= 0.1;
        }
        if (event.key == "ArrowRight") {
            x_translation += 0.1;
        }
        if (event.key == "ArrowUp") {
            y_translation += 0.1;
        }
        if (event.key == "ArrowDown") {
            y_translation -= 0.1;
        }
    });

    // 1. Specify geometry / coordinates

    var vertices = [
        -0.5, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,

        -0.5, 0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0];

    var indices = [0, 1, 2, 3, 4, 5];

    var texture_coordinates = [
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
    ];

    // 2. Set texture options

    var texture_object = gl.createTexture();

    gl.bindTexture(gl.TEXTURE_2D, texture_object);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

    // 3. Load the vertex buffer

    var vertex_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // 4. Load the index buffer

    var index_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, index_buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);

    // 5. Load the texture coordinate buffer

    var texture_coord_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, texture_coord_buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texture_coordinates), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // 6. Specify and compiler the vertex shader

    var vertex_source =
        "precision mediump float; " +
        "uniform mat4 u_translation; " +
        "uniform mat4 u_rotation; " +
        "uniform mat4 u_shear; " +
        "uniform mat4 u_scale; " +
        "attribute vec3 coordinate; " +
        "attribute vec2 texture_coordinate; " +
        "varying vec2 tex;" +
        "void main(void) {" +
        "  gl_Position = u_translation * u_rotation * u_shear * u_scale * vec4(coordinate, 1.0);" +
        "  tex = texture_coordinate; " +
        "}";

    var vertex_shader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertex_shader, vertex_source);
    var o = gl.compileShader(vertex_shader);

    // 7. Specify and compiler the fragment shader

    var fragment_source =
        "precision mediump float; " +
        "varying vec2 tex;" +
        "uniform sampler2D u_sampler;" +
        "void main(void) {" +
        "  gl_FragColor = texture2D(u_sampler, tex);" +
        "}";

    var fragment_shader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragment_shader, fragment_source);
    var o = gl.compileShader(fragment_shader);

    // 8. Create a GLSL program

    var shader_program = gl.createProgram();
    gl.attachShader(shader_program, vertex_shader);
    gl.attachShader(shader_program, fragment_shader);
    gl.linkProgram(shader_program);
    gl.useProgram(shader_program);

    // 9. Connect the variables in the program to data

    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, index_buffer);
    var coord = gl.getAttribLocation(shader_program, "coordinate");
    gl.vertexAttribPointer(coord, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(coord);

    gl.bindBuffer(gl.ARRAY_BUFFER, texture_coord_buffer);
    var texture_coord = gl.getAttribLocation(shader_program, "texture_coordinate");
    gl.vertexAttribPointer(texture_coord, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(texture_coord);

    var u_sampler = gl.getUniformLocation(shader_program, "u_sampler");
    gl.uniform1i(u_sampler, 0);

    // 10. Create the render loop

    function draw(t) {
        t /= 500;

        function transpose(a) {
            function swap(a, i, j) {
                var s = a[j * 4 + i];
                var t = a[i * 4 + j];
                a[j * 4 + i] = t;
                a[i * 4 + j] = s;
            }
            swap(a, 0, 1);
            swap(a, 0, 2);
            swap(a, 0, 3);
            swap(a, 1, 2);
            swap(a, 1, 3);
            swap(a, 2, 3);
            return a;
        }

        // Translation matrix
        var u_translation = gl.getUniformLocation(shader_program, "u_translation");
        gl.uniformMatrix4fv(
            u_translation,
            false,
            transpose([1, 0, 0, x_translation,
                       0, 1, 0, y_translation,
                       0, 0, 1, 0,
                       0, 0, 0, 1])
        );

        // Rotation matrix
        var u_rotation = gl.getUniformLocation(shader_program, "u_rotation");
        gl.uniformMatrix4fv(
            u_rotation,
            false,
            transpose([Math.cos(theta), -Math.sin(theta), 0, 0,
                       Math.sin(theta), Math.cos(theta), 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1])
        );


        // Shear matrix
        var u_shear = gl.getUniformLocation(shader_program, "u_shear");
        gl.uniformMatrix4fv(
            u_shear,
            false,
            transpose([1, x_shear, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1])
        );

        // Scale matrix
        var u_scale = gl.getUniformLocation(shader_program, "u_scale");
        gl.uniformMatrix4fv(
            u_scale,
            false,
            transpose([scale, 0, 0, 0,
                       0, scale, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1])
        );

        // 11. Render

        gl.clearColor(0.8, 0.8, 0.8, 1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.clear(gl.COLOR_BUFFER_BIT);
        gl.viewport(0,0,canvas.width, canvas.height);
        gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT, 0);
    }

    function next_frame(time) {
        draw(time);
        requestAnimationFrame(next_frame);
    }
    requestAnimationFrame(next_frame);
}

var image = new Image();
image.onload = function () {start(image);};
image.src = "pretty_sky.png";
