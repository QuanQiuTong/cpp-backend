import { defineConfig } from "vite";
import path from "path";
import vue from "@vitejs/plugin-vue";
import vueJsx from "@vitejs/plugin-vue-jsx";
import envCompatible from "vite-plugin-env-compatible";
import { createHtmlPlugin } from "vite-plugin-html";
import { viteCommonjs } from "@originjs/vite-plugin-commonjs";

// https://vitejs.dev/config/

export default defineConfig({
  resolve: {
    alias: [
      {
        find: "/^~/",
        replacement: "",
      },
      {
        find: "@",
        replacement: path.resolve(__dirname, "src"),
      },
    ],
    extensions: [".mjs", ".js", ".ts", ".jsx", ".tsx", ".json", ".vue"],
  },
  plugins: [
    vue(),
    vueJsx(),
    viteCommonjs(),
    envCompatible(),
    createHtmlPlugin({
      inject: {
        data: {
          title: "vue-soft-ui-dashboard",
        },
      },
    }),
  ],
  build: {},
  define: {
    "process.env": { ...import.meta.env },
  },
  server: {
    // /** 设置 host: true 才可以使用 Network 的形式，以 IP 访问项目 */
    // host: true, // host: "0.0.0.0"
    cors: true,
    proxy: {
        "/api": {
            target: "http://127.0.0.1:8080",
            changeOrigin: true,
            rewrite: (path) => path.replace(/^\/api/, ''),
        },
    },
  }
});

