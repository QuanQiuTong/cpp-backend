module.exports = {
  root: true,
  env: {
    node: true,
    // es2022: true
  },
  extends: ["plugin:vue/vue3-essential", "eslint:recommended", "@vue/prettier"],
 
  rules: {
    // "prettier/prettier": "off",
    // "no-console": process.env.NODE_ENV === "production" ? "warn" : "off",
    // "no-debugger": process.env.NODE_ENV === "production" ? "warn" : "off"
  }
};
